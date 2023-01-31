#include "MCTS_Sequential.h"

#include <forge/core/GameState.h>
#include <forge/search/UCB.h>

using namespace std;

namespace forge
{
	// -------------------------------- METHODS ----------------------------------

	MCTS_Node::iterator MCTS_Sequential::select(MCTS_Node::iterator curr) {
		auto & sm = m_searchMonitor;

		sm.selection.resume();
		while (curr.hasChildren()) {
			curr.toBestUCB();
		}
		sm.selection.pause();

		return curr;
	}

	void MCTS_Sequential::expand(MCTS_Node::iterator curr) {
		auto & sm = m_searchMonitor;

		sm.expansion.resume();
		curr.expand();
		sm.expansion.pause();
	}

	MCTS_Base<composite>::EvalVisits MCTS_Sequential::evaluate(MCTS_Node::iterator curr) {
		auto & sm = m_searchMonitor;

		sm.evaluation.resume();

		EvalVisits ret;

		GameState gstate;
		gstate.init(*curr);// Pass in number of children. Use more efficient overload.
		// TODO: ^^^ do we need to also pass game_history ^^^

		// --- Is Terminal or Intermediate? ---
		if (gstate.isGameOn() /*&& curr.hasChildren()*/) {
			// Intermediate Node. Evaluate using Heuristic.

			bool maximizeWhite = (*curr).position().isWhitesTurn();

			vector<const Position *> pChildren = (*curr).getChildrenPositions();

			vector<heuristic_t> evals = this->m_heuristicPtr->eval(pChildren, maximizeWhite);

			ret.eval = -(*curr).updateChildrenUCB(evals);

			ret.visits = evals.size();
		}
		else {
			// Terminal Node. Evaluate using Game State.
			bool maximizeWhite = (*curr).position().isBlacksTurn();
			ret.eval = (float) UCB::WINNING_EVAL * gstate.getValue(maximizeWhite);	// count a win as 15 pawns
			ret.visits = 1;

			(*curr).lastVisit();
		}

		if (curr.hasChildren() == false) {
			(*curr).lastVisit();
		}

		sm.evaluation.pause();

		return ret;
	}

	void MCTS_Sequential::backPropagate(MCTS_Node::iterator curr, EvalVisits ev) {
		auto & sm = m_searchMonitor;
		
		sm.backprop.resume();
		
		while (curr.isRoot() == false) {
			(*curr).update(ev.eval);//, ev.visits);	// ??? For some reason this does better with visits set to 1 ???
			ev.eval = -ev.eval;
			curr.toParent();
			(*curr).sort();
		}
		
		sm.backprop.pause();
	}

	void MCTS_Sequential::solve() {
		// --- Start ---
		m_nodeTree.root().expand();// This code is not redundant
		
		// vvvvvvvvvvv benchmarking vvvvvvvvvvvvvvvvv
		auto & sm = m_searchMonitor;
		sm.selection.reset();
		sm.evaluation.reset();
		sm.expansion.reset();
		sm.backprop.reset();
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		while (true) {
			MCTS_Node::iterator curr = m_nodeTree.root();
		
			// --- Selection ---
			curr = select(curr);
			
			// --- Expand ---
			expand(curr);
			
			// --- Evaluate ---
			EvalVisits ev = evaluate(curr);
			
			// --- BackPropagate ---
			backPropagate(curr, ev);
			
			// --- Exit Condition ---
			sm.nodeCount.add(ev.visits);
			if (sm.exitConditionReached()) {
				sm.stop();	// stop the clock so we can record exact search time.
				break;
			}
		} // while (true)
	}
} // namespace forge