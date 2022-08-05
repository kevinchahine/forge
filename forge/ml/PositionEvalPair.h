#pragma once

#include <forge/core/Position.h>
#include "forge/heuristics/HeuristicBase.h"		// for heuristic_t

#include <iostream>

namespace forge
{
	class PositionEvalPair
	{
	public:
		PositionEvalPair(const Position & position, heuristic_t eval) :
			position(position),
			eval(eval) {}
		PositionEvalPair() = default;
		PositionEvalPair(const PositionEvalPair &) = default;
		PositionEvalPair(PositionEvalPair &&) noexcept = default;
		~PositionEvalPair() noexcept = default;
		PositionEvalPair & operator=(const PositionEvalPair &) = default;
		PositionEvalPair & operator=(PositionEvalPair &&) noexcept = default;
		
	public:
		Position position;
		heuristic_t eval = 0;
	};
} // namespace forge
