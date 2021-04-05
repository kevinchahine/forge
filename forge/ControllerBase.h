#pragma once

#include "MovePositionPair.h"
#include "MoveList.h"

#include <iostream>

namespace forge
{
	class ControllerBase
	{
	public:
		virtual void reset();

		virtual MovePositionPair getMove(const Position & position) = 0;

		virtual std::string getName() = 0;

	protected:

	};
} // namespace forge