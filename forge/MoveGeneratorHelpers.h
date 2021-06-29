#pragma once

#include "Board.h"
#include "BoardSquare.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	BoardSquare findAttacker(BoardSquare attackedSquare, const Board & board, BitBoard theirs, BitBoard ours);



} // namespace forge

#include "MoveGeneratorHelpersDefinitions.h"