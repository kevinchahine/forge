#pragma once

#include "MCTS_Solver.h"

#include <thread>
#include <string> 

namespace forge
{
	class MCTS_Solver_MT : public MCTS_Solver
	{
	public:
		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "Root Parallel " + std::to_string(m_nThreads); }

	public:
		MovePositionPair solve(const Position& position);

	public:
		size_t m_nThreads = 8; // std::thread::hardware_concurrency();
	};
} // namespace forge