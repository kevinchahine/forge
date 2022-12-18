#pragma once

#include "forge/controllers/SolverBase.h"
#include "forge/search/MCTS_Node.h"

#include <set>

namespace forge
{
	template< template< typename T, typename... > class AC, typename... Args >
	class MCTS_Base : public SolverBaseTemplate<AC>
	{
	public:
		virtual void reset() override { m_nodeTree.reset(); }

		virtual MovePositionPair getMove(const Position& position) override {
			m_searchMonitor.start();

			m_nodeTree.reset();
			m_nodeTree.position() = position;

			solve();

			m_searchMonitor.stop();	// stop the clock so we can record exact search time.

			MovePositionPair bestMove = selectBestMove();

			return bestMove;
		}

		virtual std::string getName() const override { return "MCTS"; }
		
		const MCTS_Node& nodeTree() const { return m_nodeTree; }

	protected:
		virtual MovePositionPair solve() = 0;

		MovePositionPair selectBestMove() {
			MCTS_Node::iterator bestIt = m_nodeTree.root();

			//bestIt.toBestUCB();		// Stochastic selection
			//bestIt.toBestAverage();	// Best selection
			bestIt.toMostVisited();

			MovePositionPair solution {
				(*bestIt).move(),
				(*bestIt).position()
			};

			return solution;
		}

	protected:
		MCTS_Node m_nodeTree;
	};
} // namespace forge