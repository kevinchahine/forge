#pragma once

#include "forge/heuristics/HeuristicBase.h"
#include "forge/heuristics/WeightsArchive.h"

#include <iostream>
#include <vector>

namespace forge
{
	class WeightsBase
	{
	public:
		WeightsBase() = default;
		WeightsBase(const WeightsBase &) = default;
		WeightsBase(WeightsBase &&) noexcept = default;
		virtual ~WeightsBase() noexcept = default;
		WeightsBase & operator=(const WeightsBase &) = default;
		WeightsBase & operator=(WeightsBase &&) noexcept = default;

		bool operator==(const WeightsBase & rhs) const;

		virtual void serialize(WeightsArchive & ar) const = 0;
		virtual void parse(WeightsArchive & ar) = 0;

	};
} // namespace forge