#pragma once

#include "MCTS_Solver.h"

#include <thread>

namespace forge
{
	class MCTS_Solver_MT : public MCTS_Solver
	{
	public:
		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "Root Parallel"; }

	public:
		MovePositionPair solve(const Position& position);

	private:
		size_t m_nThreads = std::thread::hardware_concurrency();
	};
} // namespace forge