#pragma once

#include "forge/heuristic/Base.h"

#include <stdint.h>
#include <random>

namespace forge
{
	namespace heuristic
	{
		class Random : public Base
		{
		public:
			Random() : Base(), mydist(-1000, 1000) {}
			Random(const Random&) = default;
			Random(Random&&) noexcept = default;
			virtual ~Random() noexcept = default;
			Random& operator=(const Random&) = default;
			Random& operator=(Random&&) noexcept = default;

			virtual heuristic_t eval(const Position& pos) override;

			virtual heuristic_t eval(const Position& pos, bool whiteIsSearching = true) override;

			virtual std::unique_ptr<Base> clone() const override;	// TODO: Optimize: Why can't this be inlined. See other derivations as well.

			virtual std::string name() const override { return "Random"; }

			virtual void print(const Position& pos, std::ostream& os = std::cout) const override;

		protected:
			std::uniform_int_distribution<heuristic_t> mydist;	// units centi pawns
		};
	} // namespace heuristic
} // namespace forge