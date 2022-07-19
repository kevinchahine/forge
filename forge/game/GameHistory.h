#pragma once

#include <forge/MovePositionPair.h>

#include <queue>

namespace forge
{
	class GameHistory : public std::deque<MovePositionPair>
	{
	public:
		// Optimize:
		// emplace_back() 
		// push_back()
		// See Optimizations.txt Search GameHistory
		const MovePositionPair & current() const { return this->back(); }
		MovePositionPair& current() { return this->back(); }

		void toPGN() const;
		void fromPGN() const;
	};
} // namespace forge