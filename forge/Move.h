#pragma once

#include "BoardSquare.h"
#include "Piece.h"

namespace forge
{
	class Move
	{
	public:

	protected:
		BoardSquare from;
		BoardSquare to;
		Piece promotion;
	};
} // namespace forge