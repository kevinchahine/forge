#pragma once

#include "forge/heuristic/Base.h"

#include <iostream>
#include <memory>

namespace forge
{
	namespace heuristic
	{
		class Rollout : public Base
		{
		public:

			virtual heuristic_t eval(const Position& pos) override;

			virtual heuristic_t eval(const Position& pos, bool maximizeWhite) override;

			virtual std::vector<heuristic_t> eval(const std::vector<const Position *> & positions, bool whiteIsSearching) override;
			
			virtual std::unique_ptr<Base> clone() const override { return std::make_unique<Rollout>(*this); }

			virtual std::string name() const override { return "Rollout"; }

			virtual void print(const Position& pos, std::ostream& os = std::cout) const override { os << this->name(); }

		private:
		};
	} // namespace heuristic
} // namespace forge
