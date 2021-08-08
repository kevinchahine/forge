#pragma once

#include "Attackers.h"

namespace forge
{
	// RAY_DIRECTION_T - Direction from 'square' to an possible attacker
	//	
	// *** We don't account for pins here. ***
	// *** Tecnically a piece which is absolutely pinned can still participate in attacks ***
	// *** even if it cannot move to its victim. ***
	template<typename RAY_DIRECTION_T>
	BoardSquare Attackers::findAttackingRay(
		BoardSquare square,
		const Board & board, 
		BitBoard aggressors)	// All pieces of the attacking color.
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		// Initialize to invalid incase an attacker is not found.
		BoardSquare attacker = BoardSquare::invalid();

		// Get only the pieces which can attack in the required direction.
		// Make sure they are of the attacking color.
		// ex: If RAY_DIRECTION_T is Left, then get all Rooks and Queens.
		BitBoard directionals = board.directionals<RAY_DIRECTION_T>() & aggressors;

		// Get all squares where an attacker must exist on to effectively attack square.
		BitBoard extents = BitBoard::mask<RAY_DIRECTION_T>(square);

		// Combine into one BitBoard.
		// Now possibleAttackers is all the pieces which can attack in the specified
		// Direction.
		// ex: if RAY_DIRECTION_T is Left then
		// 'possibleAttackers' would be all the Queens and Rooks of the 'aggressors' color
		//	which exist on the same row as 'square'
		BitBoard possibleAttackers = directionals & extents;

		// Is it possible that this square is attacked from the specified direction
		// by a piece in 'possibleAttackers'?
		if (possibleAttackers.any()) {
			// Yes. There might be an attacker from 'possibleAttackers' but there might 
			// be obstacles in their way.

			// Search in specified direction for the attacker
			BoardSquare bs = square;
			while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
				bs = RAY_DIRECTION_T::move(bs);	// Increment first to skip over 'square'

				// What is on this square?
				if (possibleAttackers[bs]) {
					// --- Attacker Found ---
					attacker = bs;
					break;
				}
				else if (board.occupied()[bs]) {
					// --- Obstacle Found (occupied by some other piece) ---
					break;	// Attacker does not exist
				}
				else {
					// --- Empty square ---
					// Nothing to do here. Just keep looking for an attacker.
				}
			}
		}

		return attacker;
	}

	template<typename KNIGHT_DIRECTION_T>
	BoardSquare Attackers::findAttackingKnight(
		BoardSquare attackedSquare, 
		const Board & board, 
		BitBoard theirs)
	{
		static_assert(std::is_base_of<directions::LShape, KNIGHT_DIRECTION_T>(),
			"KNIGHT_DIRECTION_T must be derived from LShape");
		static_assert(!std::is_same<directions::LShape, KNIGHT_DIRECTION_T>(),
			"KNIGHT_DIRECTION_T must be derived from LShape and not be LShape");

		BoardSquare attacker;

		BitBoard theirKnights = board.directionals<KNIGHT_DIRECTION_T>() & theirs;
		BitBoard extents = BitBoard::mask<KNIGHT_DIRECTION_T>(attackedSquare);
		BitBoard possibleAttackers = theirKnights & extents;

		// Was their a Knight at that square
		if (possibleAttackers.any()) {
			// Yes. There might be an attacker
			attacker = KNIGHT_DIRECTION_T::move(attackedSquare);
		}
		else {
			// No. Its not possible, non of their aggressors line up with the square
			attacker.setAsInvalid();	// No attacker was found
		}

		return attacker;
	}

	template<typename RAY_DIRECTION_T>
	BitBoard Attackers::genAttackLineSegment(BoardSquare attacker, BitBoard obstacles)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		BitBoard attacked;

		while (RAY_DIRECTION_T::wouldBeInBounds(attacker)) {
			attacker = RAY_DIRECTION_T::move(attacker);

			attacked[attacker] = 1;	// attacker can attack this cell

			// Does this square have anything on it?
			if (obstacles[attacker]) {
				// Yes. It contains some type of obstacle. Either one of our or one of their pieces.
				break;
			}
		}

		return attacked;
	}
} // namespace forge