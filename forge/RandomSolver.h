#pragma once

#include "SolverBase.h"

#include <random>
#include <time.h>

namespace forge
{
	class RandomSolver : public SolverBase
	{
	public:
		virtual MovePositionPair getMove(const Position & position) override;

		virtual std::string getName() const override { return typeid(*this).name(); }

		virtual std::string getNameVariant() const override { return "Basic"; }

	protected:
		std::default_random_engine m_randomEngine{ static_cast<unsigned int>(time(nullptr)) };
	};
} // namespace forge
