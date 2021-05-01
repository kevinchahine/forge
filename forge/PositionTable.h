#pragma once

#include "HeuristicBase.h"
#include "WeightsArchive.h"

#include <array>

namespace forge
{
	class PositionTable : public std::array<std::array<heuristic_t, 8>, 8>
	{
	public:

		void serialize(WeightsArchive & ar) const;
		void parse(const WeightsArchive & ar);

	};
} // namespace forge