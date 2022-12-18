#include "MCTS_Sequential.h"

#include <forge/core/GameState.h>

using namespace std;

namespace forge
{
	MovePositionPair MCTS_Sequential::solve()
	{
		// --- Start ---
		m_nodeTree.root().expand();

		// vvvvvvvvvvv benchmarking vvvvvvvvvvvvvvvvv
		int badTraversals = 0;

		auto& sm = m_searchMonitor;
		sm.selection.reset();
		sm.evaluation.reset();
		sm.expansion.reset();
		sm.backprop.reset();
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		MCTS_Node::iterator curr = m_nodeTree.root();
		curr.expand();

		while (true) {

			// --- Selection ---
			if (curr.isLeaf()) {
				heuristic_t eval = 0;
				if ((*curr).isVisited()) {

					sm.expansion.resume();		// BENCHMARKING
					curr.expand();
					sm.expansionCount.add(1);
					sm.expansion.pause();		// BENCHMARKING

					sm.evaluation.resume();		// BENCHMARKING
					if (curr.hasChildren()) {
						// *** Intermediate Node ***
						curr.toFirstChild();	// BENCHMARKING
						//curr.toBestUCB();

						// TODO: Optimization: Since we will "eventually" evaluate all children 
						// we can optionally evaluate all children at once
						// without significantly changing the algorithms behavior.
						// This can be a good optimization when evaluations are 
						// more efficient in batches.
						bool maximizeWhite = (*curr).position().moveCounter().isBlacksTurn();
						heuristic_t eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
					}
					else {
						// *** Terminal Node ***
						bool maximizeWhite = (*curr).position().moveCounter().isBlacksTurn();
						GameState gstate;
						gstate(*curr);
						eval = 1'500 * gstate.getValue(maximizeWhite);	// count a win as 15 pawns

						(*curr).lastVisit();
					}
					sm.evaluation.pause();		// BENCHMARKING
				}
				else {
					sm.evaluation.resume();		// BENCHMARKING	
					bool maximizeWhite = (*curr).position().moveCounter().isBlacksTurn();
					eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
					sm.evaluation.pause();		// BENCHMARKING
				}

				// --- Backpropagate ---
				sm.backprop.resume();
				while (curr.isRoot() == false) {
					(*curr).update(eval);
					eval = -eval;
					curr.toParent();
				}
				
				curr = m_nodeTree.root();
				//// *** Now curr is at the root ***
				//(*curr).update(eval);	// one more time for the root
				//eval = -eval;
				
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
					curr.toBestUCB();
					sm.selection.pause();
				}
				else {
					cout << "bad visit" << endl;
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
		else {
			cout << guten::color::green << " no bad traversals Yay!!!" << endl;
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