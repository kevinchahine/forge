#pragma once

#include "HeuristicBase.h"

#include <stdint.h>
#include <random>

namespace forge
{
	class RandomHeuristic : public HeuristicBase
	{
	public:
		RandomHeuristic() : HeuristicBase(), mydist(-1000, 1000) {}
		RandomHeuristic(const RandomHeuristic &) = default;
		RandomHeuristic(RandomHeuristic &&) noexcept = default;
		virtual ~RandomHeuristic() noexcept = default;
		RandomHeuristic & operator=(const RandomHeuristic &) = default;
		RandomHeuristic & operator=(RandomHeuristic &&) noexcept = default;

		virtual heuristic_t eval(const Position & pos) const override;

		virtual std::unique_ptr<HeuristicBase> clone() const override;	// TODO: Optimize: Why can't this be inlined. See other derivations as well.

		virtual std::string name() const override { return "RandomHeuristic"; }

		virtual void print(const Position & pos, std::ostream & os = std::cout) const override;

	protected:
		std::uniform_int_distribution<heuristic_t> mydist;	// units centi pawns
	};
} // namespace forge