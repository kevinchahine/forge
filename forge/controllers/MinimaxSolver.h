#pragma once

#include "SolverBase.h"
#include "MiniMaxNode.h"
#include "Line.h"

namespace forge
{
	class MinimaxSolver : public SolverBase
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position & position) override;

		virtual std::string getName() const override { return "Minimax"; }

		virtual std::string getNameVariant() const override { return "Basic"; }

	protected:
		MovePositionPair solve(const Position & position);

	protected:
		MiniMaxNode m_nodeTree;
	}; // class MinimaxSolver
} // namespace forge