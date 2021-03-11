#pragma once

#include "Move.h"
#include "Position.h"

namespace forge
{
	class MovePositionPair
	{
	public:
		MovePositionPair() = default;
		MovePositionPair(Move move, const Position & position) :
			move(move),
			position(position) {}
		MovePositionPair(const MovePositionPair &) = default;
		MovePositionPair(MovePositionPair &&) noexcept = default;
		~MovePositionPair() noexcept = default;
		MovePositionPair & operator=(const MovePositionPair &) = default;
		MovePositionPair & operator=(MovePositionPair &&) noexcept = default;

	public:
		Move move;
		Position position;
	};
} // namespace forge