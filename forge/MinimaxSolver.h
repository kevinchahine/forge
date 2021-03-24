#pragma once

#include "SolverBase.h"
#include "Node.h"
#include "Line.h"
#include "SearchMonitor.h"

namespace forge
{
	class MinimaxSolver : public SolverBase
	{
	public:
		virtual void reset() override;

		virtual Move getMove(const Position & position) override;

		virtual std::string getName() override;

	protected:

		Move solve(const Position & position);

	protected:

		Node m_nodeTree;

		SearchMonitor m_searchMonitor;
	};
} // namespace forge