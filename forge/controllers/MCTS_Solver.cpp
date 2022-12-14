#include "MCTS_Solver.h"

#include <forge/core/GameState.h>

using namespace std;

namespace forge
{
	void MCTS_Solver::reset()
	{
		m_nodeTree.reset();
		curr = m_nodeTree.root();
	}

	MovePositionPair MCTS_Solver::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);

		this->searchMonitor().print();

		return bestMove;
	}

	MovePositionPair MCTS_Solver::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();

		bool maximize = m_nodeTree.position().moveCounter().isWhitesTurn();

		//bestIt.toBestUCB(maximize);	// Stochastic selection
		//bestIt.toBestAverage(maximize);		// Best selection
		bestIt.toMostVisited();

		MovePositionPair solution {
			(*bestIt).move(),
			(*bestIt).position()
		};

		return solution;
	}

	MovePositionPair MCTS_Solver::solve(const Position& position)
	{
		int badTraversals = 0;

		// --- Start ---

		auto& sm = m_searchMonitor;
		sm.start();

		// vvvvvvvvvvv benchmarking vvvvvvvvvvvvvvvvv
		sm.selection.reset();
		sm.evaluation.reset();
		sm.expansion.reset();
		sm.backprop.reset();
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		bool maximizeWhite = position.moveCounter().isWhitesTurn();

		m_nodeTree.reset();
		m_nodeTree.position() = position;
		curr = m_nodeTree.root();
		curr.expand();

		while (true) {
			
			// --- Selection ---
			if (curr.isLeaf()) {
				heuristic_t eval = 0;
				
				if ((*curr).isVisited()) {
					
					sm.expansion.resume();		// BENCHMARKING
					curr.expand();
					sm.expansion.pause();		// BENCHMARKING

					sm.evaluation.resume();		// BENCHMARKING
					if (curr.hasChildren()) {
						// *** Intermediate Node ***
						curr.toFirstChild();	// BENCHMARKING
					
						// TODO: Optimization: Since we will "eventually" evaluate all children 
						// we can optionally evaluate all children at once
						// without significantly changing the algorithms behavior.
						// This can be a good optimization when evaluations are 
						// more efficient in batches.
						heuristic_t eval = this->m_heuristicPtr->eval((*curr).position());
					}
					else {
						// *** Terminal Node ***
						GameState gstate;
						gstate(*curr);
						eval = 1'500 * gstate.getValue(true);	// count a win as 15 pawns
						(*curr).lastVisit();
					}
					sm.evaluation.pause();		// BENCHMARKING
				}
				else {
					sm.evaluation.resume();		// BENCHMARKING	
					eval = this->m_heuristicPtr->eval((*curr).position());
					sm.evaluation.pause();		// BENCHMARKING
				}
				
				// --- Backpropagate ---
				sm.backprop.resume();
				while (curr.isRoot() == false) {
					(*curr).update(eval);
					curr.toParent();
				}
				
				// *** Now curr is at the root ***
				(*curr).update(eval);	// one more time for the root

				// --- Check stopping condition ---
				sm.nodeCount++;
				if (sm.exitConditionReached()) {
					sm.stop();	// stop the clock so we can record exact search time.
					break;
				}
				sm.backprop.pause();
			}
			else {
				sm.selection.resume();
				// --- Move DOWN the tree ---
				if ((*curr).isPruned() == false && curr.hasChildren()) {
					bool isWhitesTurn = (*curr).position().moveCounter().isWhitesTurn();
					curr.toBestUCB(isWhitesTurn);
					sm.selection.pause();
				}
				else {
					// We reached a terminal node (expended but without children).
					// The only thing we can do here is go back to the root and continue the search.
					(*curr).nBadVisits++;	// TODO: remove
					badTraversals++;		// TODO: remove. remove field from class also

					curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.
					
					sm.selection.pause();
					if (sm.exitConditionReached()) {
						sm.stop();	// stop the clock so we can record exact search time.
						break;
					}

					// *******************************
					// TODO: Optimize: This code is very inefficient. When all nodes in a subtree are terminal 
					//	(win/lose/draw no children even when expanded)
					//	continuing to search this subtree will result in no added computation. 
					//	A possible optimization is to mark each node as being fully searched when all its
					//	leaf nodes are terminal.
					//	Or we can prune subtrees when they are fully searched. (Might not be the best option
					//	for multithreading. Also we are deleting data we can use in the next search.)
					// *******************************
				}
			}
		}

		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvv REMOVE VVVVVVVVVVVVVVVVVVVVVVV
		cout << guten::color::lightred;

		if (badTraversals > 0) {
			cout << badTraversals << " bad traversals discovered" << endl;
		}

		curr = m_nodeTree.root();

		stack<MCTS_Node*> frontier;

		frontier.push(&m_nodeTree);

		while (frontier.size()) {
			const MCTS_Node* top = frontier.top();
			frontier.pop();

			if (top->nBadVisits > 0) {
				cout << top->nBadVisits << "\tbad visits." << endl;

				cout << "----" << endl;
			}

			for (const auto& child : top->children()) {
				frontier.push(child.get());
			}
		}

		cout << guten::color::white;

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		return selectBestMove();
	}
} // namespace forge