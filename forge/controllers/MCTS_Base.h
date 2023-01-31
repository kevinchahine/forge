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

		class NodeEvalVisits
		{
		public:
			MCTS_Node node;
			float eval = 0.0f;
			size_t visits = 0;
		};

	public:
		virtual void reset() override { m_nodeTree.reset(); }

		virtual MovePositionPair getMove(const Position& position) override {
			// vvvvvvvvvvv benchmarking vvvvvvvvvvvvvvvvv
			auto & sm = m_searchMonitor;
			sm.selection.reset();
			sm.evaluation.reset();
			sm.expansion.reset();
			sm.backprop.reset();
			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

			m_searchMonitor.start();

			m_nodeTree.reset();
			m_nodeTree.position() = position;

			solve();

			m_searchMonitor.stop();	// stop the clock so we can record exact search time.

			MovePositionPair bestMove = selectBestMove();
			//MovePositionPair bestMove = selectBestStochasticMove();

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

	public:
		// Evaluation for a win in units of centi pawns
		const static heuristic_t WIN_EVAL = 1'500;

	protected:
		MCTS_Node m_nodeTree;
	};
} // namespace forge