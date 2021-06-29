#pragma once

#include "MoveGeneratorHelpers.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	BoardSquare findAttacker(BoardSquare attackedSquare, const Board & board, BitBoard theirs, BitBoard ours)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		BoardSquare attacker;

		BitBoard theirAttackers = board.directionals<RAY_DIRECTION_T>() & theirs;
		BitBoard extents = BitBoard::mask<RAY_DIRECTION_T>(attackedSquare);
		BitBoard possibleAttackers = theirAttackers & extents;

		// Is it possible that this square is attacked from the specified direction.
		if (possibleAttackers.any()) {
			// Yes. There might be an attacker

			// Search in specified direction for a possible attacker
			BoardSquare bs = attackedSquare;

			while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
				bs = RAY_DIRECTION_T::move(bs);

				// What is on this square
				if (possibleAttackers[bs]) {
					// It is one of there attackers which can attack in our direction.
					attacker = bs;
					break;
				}
				else if (ours[bs] || theirs[bs]) {
					// It is our pieces, which means it is blocking a possible attacker. 
					// --- Or ---
					// It is one of their other pieces, but one which can't attack in our direction.
					attacker.setAsInvalid();
					break;
				}
			}
		}
		else {
			// No. Its not possible, non of their attackers line up with the square
			attacker.setAsInvalid();	// No attacker was found
		}

		return attacker;
	}
} // namespace forge