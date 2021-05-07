#pragma once

#include "SolverBase.h"
#include "Node.h"
#include "Line.h"

namespace forge
{
	class MinimaxSolver : public SolverBase
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position & position) override;

		virtual std::string getName() override;

	protected:
		MovePositionPair solve(const Position & position);

	protected:
		Node m_nodeTree;
	};
} // namespace forge