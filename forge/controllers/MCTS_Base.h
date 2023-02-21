#pragma once

#include "forge/controllers/SolverBase.h"
#include "forge/search/MCTS_Node.h"

#include <set>

namespace forge
{
	template< template< typename T, typename... > class AC, typename... Args >
	class MCTS_Base : public SolverBaseTemplate<AC>
	{
	protected:// ------------------------- NESTED CLASS --------------------------
		class EvalVisits
		{
		public:
			float eval = 0.0f;
			size_t visits = 0;
		};

		class NodeItEvalVisits
		{
		public:
			MCTS_Node::iterator nodeIt;
			float eval = 0.0f;
			size_t visits = 0;
		};

	public:
		virtual void reset() override { m_nodeTree.reset(); }

		virtual MovePositionPair getMove(const Position& position) override {
			m_searchMonitor.start();

			m_nodeTree.reset();
			m_nodeTree.position() = position;

			solve();

			m_searchMonitor.stop();	// stop the clock so we can record exact search time.

			MovePositionPair bestMove = 
				selectBestMove();
			//selectBestStochasticMove();

			m_history.push_back(bestMove);

			return bestMove;
		}

		virtual std::string getName() const override { return "MCTS"; }
		
		const MCTS_Node& nodeTree() const { return m_nodeTree; }

	protected:
		virtual void solve() = 0;

		MovePositionPair selectBestMove() {
			MCTS_Node::iterator bestIt = m_nodeTree.root();

			bestIt.toBestAverage();
			//bestIt.toMostVisited();

			MovePositionPair solution {
				(*bestIt).move(),
				(*bestIt).position()
			};

			return solution;
		}

		MovePositionPair selectBestStochasticMove() {
			MCTS_Node::iterator bestIt = m_nodeTree.root();

			bestIt.toBestStochastic();

			MovePositionPair solution{
				(*bestIt).move(),
				(*bestIt).position()
			};

			return solution;
		}

	protected:
		MCTS_Node::iterator select(MCTS_Node::iterator curr) {
			while (curr.hasChildren()) {
				curr.toBestUCB();
			}

			return curr;
		}

		void expand(MCTS_Node::iterator curr) {
			curr.expand();
		}

		EvalVisits evaluate(MCTS_Node::iterator curr) {
			EvalVisits ret;

			// Check game state of current node (win/loss/draw)
			// This determines how we are to evaluate it.
			GameState gstate;
			gstate.init(*curr);// Pass in number of children. Use more efficient overload.
			// TODO: ^^^ do we need to also pass game_history ^^^

			// --- Is Terminal or Intermediate? ---
			if (gstate.isGameOn() && curr.hasChildren()) {
				// Intermediate Node. Evaluate using Heuristic.

				// Does curr belong to the maximizing or minimizing player?
				bool maximizeWhite = (*curr).position().isWhitesTurn();

				// Evaluate all the children nodes all at once.
				vector<const Position *> pChildren = (*curr).getChildrenPositions();

				vector<heuristic_t> evals = this->m_heuristicPtr->eval(pChildren, maximizeWhite);

				// Now update the UCB scores of the children nodes with the new evals
				ret.eval = -(*curr).updateChildrenUCB(evals);

				(*curr).sort();

				// TODO: Is it better to make this 1 (nVisits)
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

			return ret;
		}

		void backPropagate(MCTS_Node::iterator begin, MCTS_Node::iterator end, EvalVisits ev) {
			while (begin != end) {
				(*begin).update(ev.eval, ev.visits);	// ??? For some reason this does better with visits set to 1 ???
				begin.toParent();
				(*begin).sort();
				ev.eval = -ev.eval;
			}

			// Update one more time for the end node (usually the root node)
			if (begin.isRoot())
				(*begin).updateRoot(ev.eval, ev.visits);// update t and n values of end node without the UCB score
		}

	public:
		// Evaluation for a win in units of centi pawns
		const static heuristic_t WIN_EVAL = 1'500;

	protected:
		MCTS_Node m_nodeTree;
	};
} // namespace forge