#pragma once

#include "forge/controllers/SolverBase.h"
#include "forge/search/MCTS_Node.h"

#include <set>

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>

namespace forge
{
	class MCTS_Solver_MT : public SolverBase<boost::atomic>
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "MultiThreaded"; }

		///const MCTS_Node& nodeTree() const { return m_nodeTree; }

	protected:

		// TODO: Could make this constant
		MovePositionPair selectBestMove();

		void solveOneThread();

	public:
		MovePositionPair solve(const Position& position);

	protected:
		MCTS_Node m_nodeTree;
	};
} // namespace forge