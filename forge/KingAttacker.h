#pragma once

#include "Board.h"
#include "BoardSquare.h"
#include <boost/container/static_vector.hpp>

namespace forge
{
	// A simple data type that stores information about a piece that is attacking a King.
	class KingAttacker
	{
	public:
		// Square that the attacker is standing on
		BoardSquare square;

		// BitBoard containing a line of 1s between the King(exclusive) and Attacker(inclusive)
		// Square where king stands is a 0
		// Square where attacker stands is a 1
		// Squares inbetween are 1s also.
		// If attacker is a Knight or Pawn then its square will contain the only 1
		BitBoard lineOfFire;

		// Direction of the attacker from the Kings perspective.
		// Can be a Ray or Knight direction
		directions::Direction dir;
	};
} // namespace forge