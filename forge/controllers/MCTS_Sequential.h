#pragma once

#include "forge/controllers/MCTS_Base.h"

#include "forge/util/composite.h"

namespace forge
{
	class MCTS_Sequential : public MCTS_Base<forge::composite>
	{
	public:
		virtual std::string getNameVariant() const override { return "Sequential"; }

	public:
		virtual MovePositionPair solve() override;
	};
} // namespace forge