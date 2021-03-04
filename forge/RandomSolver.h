#pragma once

#include "SolverBase.h"

#include <random>
#include <time.h>

namespace forge
{
	class RandomSolver : public SolverBase
	{
	public:
		virtual Move getMove(const Position & position) override;

	protected:
		std::default_random_engine m_randomEngine{ time(nullptr) };
	};
} // namespace forge
