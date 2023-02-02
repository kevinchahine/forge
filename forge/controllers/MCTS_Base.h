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
				//#define _DEBUG 
				if ((*curr).isVisited()) {
					cout << "Error: Terminal node is revisited." << endl;
				}
				//#endif // _DEBUG

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
				(*begin).update(ev.eval);//, ev.visits);	// ??? For some reason this does better with visits set to 1 ???
				ev.eval = -ev.eval;
				begin.toParent();
				(*begin).sort();
			}
		}

	public:
		// Evaluation for a win in units of centi pawns
		const static heuristic_t WIN_EVAL = 1'500;

	protected:
		MCTS_Node m_nodeTree;
	};
} // namespace forge