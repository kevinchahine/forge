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
		MovePositionPair solve(const Position& position);
	};
} // namespace forge