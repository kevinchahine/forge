#include "MCTS_Sequential.h"

#include <forge/core/GameState.h>
#include <forge/search/UCB.h>

using namespace std;

namespace forge
{
	// -------------------------------- METHODS ----------------------------------

	MCTS_Node::iterator MCTS_Sequential::select(MCTS_Node::iterator curr) {
		auto & sm = m_searchMonitor;

		// --- Selection ---
		sm.selection.resume();

		// --- Move DOWN the tree ---
		while (curr.isExpanded()) {
			if (curr.hasChildren()) {
				curr.toBestUCB();
			}
			else {
				cout << "bad visit" << endl;
				// We reached a terminal node (expended but without children).
				
				curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.

				if (sm.exitConditionReached()) {
					break;
				}
			} // if (curr.isExpanded())
		} // while (true)

		sm.selection.pause();

		return curr;
	}

	MCTS_Base<composite>::EvalVisitsPair MCTS_Sequential::expandAndEvaluate(MCTS_Node::iterator curr) {
		auto & sm = m_searchMonitor;

		EvalVisitsPair evalVisits;

		sm.expansion.resume();
		curr.expand();
		sm.expansion.pause();

		sm.evaluation.resume();

		GameState gstate;
		gstate.init(*curr);// Pass in number of children. Use more efficient overload.
		
		// --- Is Terminal or Intermediate? ---
		if (gstate.isGameOn() /*&& curr.hasChildren()*/) {
			bool maximizeWhite = (*curr).position().isWhitesTurn();

			vector<const Position *> pChildren = (*curr).getChildrenPositions();

			vector<heuristic_t> evals = this->m_heuristicPtr->eval(pChildren, maximizeWhite);

			evalVisits.eval = -(*curr).updateChildrenUCB(evals);

			evalVisits.visits = evals.size();
		}
		else {
			// Terminal Node. Evaluate using Game State.
			bool maximizeWhite = (*curr).position().isBlacksTurn();

			evalVisits.eval = (float) UCB::WINNING_EVAL * gstate.getValue(maximizeWhite);	// count a win as 15 pawns

			evalVisits.visits = 1;

			(*curr).lastVisit();
		}

		// TODO: This code might be redundant.
		// If this node is terminal, we don't want to visit it again.
		if (curr.hasChildren() == false) {
			(*curr).lastVisit();
		}
		sm.evaluation.pause();

		return evalVisits;
	}

	void MCTS_Sequential::backPropagate(MCTS_Node::iterator curr, const EvalVisitsPair & evalVisits) {
		auto & sm = m_searchMonitor;

		sm.backprop.resume();

		heuristic_t eval = evalVisits.eval;

		while (curr.isRoot() == false) {
			(*curr).update(eval);//, evalVisits.visits);
			eval = -eval;
			curr.toParent();
			(*curr).sort();
		}

		// *** Now curr is at the root ***

		sm.nodeCount.add(evalVisits.visits);

		sm.backprop.pause();
	}

	void MCTS_Sequential::solve() {
		// --- Start ---
		m_nodeTree.root().expand();// TODO: this might be redundant

		// Search until exit condition is reached
		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Start traversal at Root ---
			MCTS_Node::iterator curr = m_nodeTree.root();

			// --- 2.) Select "Best" Leaf ---
			curr = this->select(curr);

			// --- 3.) Expand and Evaluate "Best" Leaf ---
			EvalVisitsPair evalVisits = this->expandAndEvaluate(curr);

			// --- 4.) Back Propagate Evals to Root ---
			this->backPropagate(curr, evalVisits);
		}
	}
} // namespace forge