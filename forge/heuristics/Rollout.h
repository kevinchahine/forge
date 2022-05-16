#pragma once

#include "forge/heuristics/HeuristicBase.h"

#include <iostream>
#include <memory>

namespace forge
{
	namespace heuristics 
	{
		class Rollout : public HeuristicBase
		{
		public:

			virtual heuristic_t eval(const Position& pos) override;

			virtual std::unique_ptr<HeuristicBase> clone() const override { return std::make_unique<Rollout>(*this); }

			virtual std::string name() const override { return "Rollout"; }

			virtual void print(const Position& pos, std::ostream& os = std::cout) const override { os << this->name(); }

		private:
		};
	} // namespace heuristics
} // namespace forge
