#pragma once

#include <forge/Position.h>
#include <forge/MoveList.h>

namespace forge
{
	class ViewBase
	{
	public:
		virtual void show(const Position & pos) = 0;

		// move - move that brought us to this position.
		// pos - represents the current game.
		virtual void show(const Position & pos, const Move & move);

		virtual void show(const Position & pos, const MoveList & validMoves);

	protected:

	};
} // namespace forge