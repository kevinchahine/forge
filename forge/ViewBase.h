#pragma once

#include "Position.h"

namespace forge
{
	class ViewBase
	{
	public:

		virtual void highlight(const BoardSquare & cell) = 0;

		// move - move that brought us to this position.
		// pos - represents the current game.
		///virtual void setPieces(const Move & move, const Position & pos) = 0;

		// move - move that brought us to this position.
		// pos - represents the current game.
		virtual void show(const Move & move, const Position & pos) = 0;

	protected:

	};
} // namespace forge