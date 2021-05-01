#pragma once

#include <iostream>
#include <vector>

namespace forge
{
	class WeightsBase
	{
	public:
		WeightsBase() = default;
		//WeightsBase(size_t nWeights) : std::vector<int>(nWeights) {}
		WeightsBase(const WeightsBase &) = default;
		WeightsBase(WeightsBase &&) noexcept = default;
		virtual ~WeightsBase() noexcept = default;
		WeightsBase & operator=(const WeightsBase &) = default;
		WeightsBase & operator=(WeightsBase &&) noexcept = default;

	};
} // namespace forge