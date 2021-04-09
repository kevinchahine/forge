#pragma once

#include "Position.h"

#include <queue>

namespace forge
{
	class GameHistory : public std::deque<Position>
	{
	public:

		const Position & current() const { return this->back(); }
		Position & current() { return this->back(); }

	};
} // namespace forge