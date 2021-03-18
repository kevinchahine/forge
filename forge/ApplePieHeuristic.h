#pragma once

#include "HeuristicBase.h"

namespace forge
{
	class ApplePieHeuristic : public HeuristicBase
	{
	public:
		virtual heuristic_t eval(const Position & pos) override;

		virtual std::unique_ptr<HeuristicBase> clone() const override;

	};
} // namespace forge