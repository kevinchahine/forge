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
		
		searchMonitor().print();

		return bestMove;
	}
	
	MovePositionPair MCTS_Solver_MT::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();
		
		//bestIt.toBestUCB(maximize);	// Stochastic selection
		//bestIt.toBestAverage(maximize);		// Best selection
		
		MovePositionPair solution{
			(*bestIt).move(),
			(*bestIt).position()
		};
		
		return solution;
	}
	
	void MCTS_Solver_MT::solveOneThread()
	{
		//MCTS_Node::iterator curr = m_nodeTree.root();
		//
		//while (true) {
		//	// --- Selection ---
		//	if (curr.isLeaf()) {
		//		heuristic_t eval = 0;
		//		
		//		if ((*curr).mutex().try_lock()) { // ~~~~~~~~~~~~~~~~ Lock ~~~~
		//			
		//			// --- 2nd visit? ---
		//			if ((*curr).isVisited()) { // Implies curr is a leaf()
		//				
		//				curr.expand();
		//				
		//				if (curr.hasChildren()) {
		//					MCTS_Node::iterator parent = curr;
		//					
		//					curr.toFirstChild();
		//					
		//					// TODO: Optimization: Since we will "eventually" evaluate all children 
		//					// we can optionally evaluate all children at once
		//					// without significantly changing the algorithms behavior.
		//					// This can be a good optimization when evaluations are 
		//					// more efficient in batches.
		//					eval = this->m_heuristicPtr->eval((*curr).position());
		//					
		//					// *** We have completely discovered and evaluated a new node ***
		//					m_searchMonitor.nodeCount.add(1);	// Atomic: nodeCount++;
		//					
		//					// --- Backpropagate One ---
		//					// Backpropagate just this node so that lurking threads will know that 
		//					// this node has been visited
		//					(*curr).update(eval);
		//					curr = parent;
		//					(*curr).update(eval);
		//					(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//				}
		//				else {
		//					GameState gstate;
		//					gstate(*curr);
		//					eval = 1'500 * gstate.getValue(true);	// count a win a 15 pawns
		//					(*curr).update(eval);
		//					(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//				}
		//			}
		//			// --- 1st visit?  ---
		//			else {
		//				eval = this->m_heuristicPtr->eval((*curr).position());
		//
		//				// *** We have completely discovered and evaluated a new node ***
		//				m_searchMonitor.nodeCount.add(1);	// Atomic: nodeCount++;
		//
		//				(*curr).update(eval);	// Indicates that this node has now been visited once
		//				(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//				curr.toParent();
		//			}
		//		}
		//		else {
		//			// Go back to root and start search again.
		//			curr = m_nodeTree.root();
		//			
		//			if (m_searchMonitor.exitConditionReached()) {
		//				// ~~~ Its possible to overstep the node count and ~~~ 
		//				// ~~~ search depth limit even with atomics. But ~~~
		//				// ~~~ that is not bad ~~~
		//				break;
		//			}
		//			else {
		//				continue;	// continue to top of loop 
		//			}
		//		}
		//		
		//		// --- Backpropagate ---
		//		while (curr.isRoot() == false) {
		//			(*curr).mutex().lock();
		//			(*curr).update(eval);			// !!! CS
		//			(*curr).mutex().unlock();
		//			curr.toParent();				// !!! Must be outside CS to unlock properly
		//		}
		//		
		//		// *** Now curr is at the root ***
		//		(*curr).mutex().lock();
		//		(*curr).update(eval);	// one more time for the root	// !!! CS 
		//		(*curr).mutex().unlock();
		//		
		//		// --- Check stopping condition ---
		//		if (m_searchMonitor.exitConditionReached()) {
		//			// ~~~ Its possible to overstep the node count and ~~~ 
		//			// ~~~ search depth limit even with atomics. But ~~~
		//			// ~~~ that is not bad ~~~
		//			break;
		//		}
		//	}
		//	else /* non leaf */ { 
		//		// ~~~ No expansions will occure at this node ~~~
		//		
		//		// --- Move DOWN the tree ---
		//		(*curr).mutex().lock();
		//		bool hasChildren = curr.hasChildren();
		//		(*curr).mutex().unlock();
		//		
		//		if (hasChildren) {
		//			// ~~~ Synchronization is required here to maintain proper search order ~~~
		//			// ~~~ But no synchronization is permisible to avoid synchronization ~~~
		//			// ~~~ overhead. ~~~
		//			bool isWhitesTurn = (*curr).position().moveCounter().isWhitesTurn();
		//
		//			curr.toBestUCB(isWhitesTurn);
		//		}
		//		else {
		//			
		//			// We reached a terminal node (stub: expended but without children).
		//			// The only useful thing we can do here is go back to the root and 
		//			// continue the search.
		//			curr = m_nodeTree.root();	// Creates an infinite loop if we don't check the exit condition.
		//			
		//			if (m_searchMonitor.exitConditionReached()) {
		//				break;
		//			}
		//			else {
		//				continue;
		//			}
		//			// *******************************
		//			// TODO: Optimize: This code is very inefficient. When all nodes in a subtree are terminal 
		//			//	(win/lose/draw no children even when expanded)
		//			//	continuing to search this subtree will result in no added computation. 
		//			//	A possible optimization is to mark each node as being fully searched when all its
		//			//	leaf nodes are terminal.
		//			//	Or we can prune subtrees when they are fully searched. (Might not be the best option
		//			//	for multithreading. Also we are deleting data we can use in the next search.)
		//			// *******************************
		//		}
		//	}
		//}
	}
	
	MovePositionPair MCTS_Solver_MT::solve(const Position& position)
	{
		// --- Start ---
		m_searchMonitor.start();
		
		m_nodeTree.reset();
		m_nodeTree.position() = position;
		m_nodeTree.root().expand();
		
		// ~~~ Concurrent Section ~~~

		std::vector<boost::thread> pool;
		const size_t nThreads = (m_nThreads == 0 ? 3 /*boost::thread::hardware_concurrency()*/ : m_nThreads);
		pool.reserve(nThreads);

		for (size_t t = 0; t < nThreads; t++) {
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