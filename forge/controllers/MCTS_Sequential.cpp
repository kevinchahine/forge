#include "MCTS_Sequential.h"

#include <forge/core/GameState.h>
#include <forge/search/UCB.h>

using namespace std;

namespace forge
{
	// -------------------------------- METHODS ----------------------------------

	void MCTS_Sequential::solve() {
		// --- Start ---
		m_nodeTree.root().expand();
		MCTS_Node::iterator curr = m_nodeTree.root();
		//curr.expand();

		// vvvvvvvvvvv benchmarking vvvvvvvvvvvvvvvvv
		int badTraversals = 0;

		auto & sm = m_searchMonitor;
		sm.selection.reset();
		sm.evaluation.reset();
		sm.expansion.reset();
		sm.backprop.reset();
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		while (true) {
			float eval = 0.0f;
			size_t nEvals = 0;

			// --- Is Expanded? ---
			if (curr.isExpanded()) {
				// Yes node has been Expanded. Continue with selection.

				// --- Selection ---
				sm.selection.resume();

				// --- Move DOWN the tree ---
				if (curr.hasChildren()) {
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
			else {
				// No node is still Fresh
				curr.expand();

				GameState gstate;
				gstate.init(*curr);// Pass in number of children. Use more efficient overload.

				// --- Is Terminal or Intermediate? ---
				if (gstate.isGameOn() /*&& curr.hasChildren()*/) {
					// Intermediate Node. Evaluate using Heuristic.

					bool maximizeWhite = (*curr).position().isWhitesTurn();

					vector<const Position *> pChildren = (*curr).getChildrenPositions();

					vector<heuristic_t> evals = this->m_heuristicPtr->eval(pChildren, maximizeWhite);

					eval = -(*curr).updateChildrenUCB(evals);

					nEvals = evals.size();
				}
				else {
					// Terminal Node. Evaluate using Game State.
					bool maximizeWhite = (*curr).position().isBlacksTurn();
					eval = (float) UCB::WINNING_EVAL * gstate.getValue(maximizeWhite);	// count a win as 15 pawns
					nEvals = 1;

					(*curr).lastVisit();
				}

				if (curr.hasChildren() == false) {
					(*curr).lastVisit();
				}
				
				// --- Backpropagate ---
				sm.backprop.resume();
				while (curr.isRoot() == false) {
					(*curr).update(eval);
					eval = -eval;
					curr.toParent();
					(*curr).sort();
				}

				//curr = m_nodeTree.root();

				// *** Now curr is at the root ***

				// --- Check stopping condition ---
				sm.nodeCount.add(nEvals);
				if (sm.exitConditionReached()) {
					sm.stop();	// stop the clock so we can record exact search time.
					break;
				}
				sm.backprop.pause();
			}
		} // end while(true)
	}
} // namespace forge