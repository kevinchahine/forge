#include "MCTS_Solver_MT.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class

using namespace std;

namespace forge
{
	void MCTS_Solver_MT::reset()
	{
		m_nodeTree.reset();
	}

	MovePositionPair MCTS_Solver_MT::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);
		
		this->m_searchMonitor.print();

		return bestMove;
	}

	MovePositionPair MCTS_Solver_MT::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();
		
		bool maximize = m_nodeTree.position().moveCounter().isWhitesTurn();
		
		//bestIt.toBestUCB(maximize);	// Stochastic selection
		bestIt.toBestAverage(maximize);		// Best selection
		
		MovePositionPair solution{
			(*bestIt).move(),
			(*bestIt).position()
		};
		
		return solution;
	}
	
	void MCTS_Solver_MT::solveOneThread()
	{
		MCTS_Node::iterator curr = m_nodeTree.root();

		//bool maximizeWhite = (*curr).position().moveCounter().isWhitesTurn();
		
		while (true) {
		
			// --- Selection ---
			if (curr.isLeaf()) {
				heuristic_t eval = 0;
				
				if ((*curr).mutex().try_lock()) {
					// --- 2nd visit? ---
					if ((*curr).isVisited()) { // Implies curr is a leaf()
						
						curr.expand();
						
						if (curr.hasChildren()) {
							// TODO: Optimization: Since we will "eventually" evaluate all children 
							// we can optionally evaluate all children at once
							// without significantly changing the algorithms behavior.
							// This can be a good optimization when evaluations are 
							// more efficient in batches.
							heuristic_t eval = this->m_heuristicPtr->eval((*curr).position());
						}
						else {
							GameState gstate;
							gstate(*curr);
							eval = 1'500 * gstate.getValue(true);	// count a win a 15 pawns
						}
					}
					// --- 1st visit?  ---
					else {
						eval = this->m_heuristicPtr->eval((*curr).position());
					}
					
					(*curr).mutex().unlock();
				}
				else {
					// Go back to root and start search again.
					//(*curr).si
					curr = m_nodeTree.root();
				
					if (m_searchMonitor.exitConditionReached()) {
						// ~~~ Its possible to overstep the node count and ~~~ 
						// ~~~ search depth limit even with atomics. But ~~~
						// ~~~ that is not bad ~~~
						break;
					}
				}

				// --- Backpropagate ---
				while (curr.isRoot() == false) {
					(*curr).mutex().lock();
					(*curr).update(eval);			// !!! CS
					(*curr).mutex().unlock();
					curr.toParent();				// !!! Must be outsize CS to unlock properly
				}
				
				// *** Now curr is at the root ***
				(*curr).mutex().lock();
				(*curr).update(eval);	// one more time for the root	// !!! CS 
				(*curr).mutex().unlock();

				// --- Check stopping condition ---
				m_searchMonitor.nodeCount.add(1);	// Atomic: nodeCount++;
				if (m_searchMonitor.exitConditionReached()) {
					// ~~~ Its possible to overstep the node count and ~~~ 
					// ~~~ search depth limit even with atomics. But ~~~
					// ~~~ that is not bad ~~~
					break;
				}
			}
			else /* non leaf */ { 
				// ~~~ No expansions will occure at this node ~~~
				
				// --- Move DOWN the tree ---
				if (curr.hasChildren()) {
					// ~~~ Synchronization is required here to maintain proper search order ~~~
					// ~~~ But no synchronization is permisible to avoid synchronization ~~~
					// ~~~ overhead. ~~~
					bool isWhitesTurn = (*curr).position().moveCounter().isWhitesTurn();
					curr.toBestUCB(isWhitesTurn);
				}
				else {
					// We reached a terminal node (stub: expended but without children).
					// The only useful thing we can do here is go back to the root and 
					// continue the search.
					curr = m_nodeTree.root();	// Creates an infinite loop if we don't check the exit condition.
					
					// TODO: Optimize: This code is very inefficient. When all nodes in a subtree are terminal 
					//	(win/lose/draw no children even when expanded)
					//	continuing to search this subtree will result in no added computation. 
					//	A possible optimization is to mark each node as being fully searched when all its
					//	leaf nodes are terminal.
					//	Or we can prune subtrees when they are fully searched. (Might not be the best option
					//	for multithreading. Also we are deleting data we can use in the next search.)
					if (m_searchMonitor.exitConditionReached()) {
						break;
					}
				}
			}
		}
	}
	
	MovePositionPair MCTS_Solver_MT::solve(const Position& position)
	{
		// --- Start ---
		m_searchMonitor.timer.expires_from_now(chrono::seconds(4));
		m_searchMonitor.start();
		
		m_nodeTree.reset();
		m_nodeTree.position() = position;
		m_nodeTree.root().expand();
		
		// ~~~ Concurrent Section ~~~

		std::vector<boost::thread> pool;
		pool.reserve(boost::thread::hardware_concurrency());

		for (size_t t = 0; t < boost::thread::hardware_concurrency(); t++) {
			pool.push_back(
				boost::thread{ 
					[this, &position]() { this->solveOneThread(); } 
				}
			);
		}

		for (boost::thread& t : pool) {
			t.join();
		}

		// ~~~ End of Concurrent Section ~~~

		m_searchMonitor.stop();	// stop the clock so we can record exact search time.

		return selectBestMove();
	}
} // namespace forge