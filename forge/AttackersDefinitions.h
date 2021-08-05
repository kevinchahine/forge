#pragma once

#include "Attackers.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	BoardSquare Attackers::findAttackingRay(
		BoardSquare attackedSquare, 
		const Board & board, 
		BitBoard theirs, 
		BitBoard ours)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		BoardSquare attacker = BoardSquare::invalid();

		BitBoard theirAttackers = board.directionals<RAY_DIRECTION_T>() & theirs;
		BitBoard extents = BitBoard::mask<RAY_DIRECTION_T>(attackedSquare);
		BitBoard possibleAttackers = theirAttackers & extents;

		// Is it possible that this square is attacked from the specified direction?
		if (possibleAttackers.any()) {
			// Yes. There might be an attacker

			// Search in specified direction for a possible attacker
			BoardSquare bs = attackedSquare;

			while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
				bs = RAY_DIRECTION_T::move(bs);

				// What is on this square?
				if (possibleAttackers[bs]) {
					// It is one of there attackers which can attack in our direction.
					attacker = bs;
					break;
				}
				else if (ours[bs] || theirs[bs]) {
					// It is our pieces, which means it is blocking a possible attacker. 
					// --- Or ---
					// It is one of their other pieces, but one which can't attack in our direction.
					//attacker.setAsInvalid();	
					break;
				}
				/* else { // It must be empty } */
			}
		}
		else {
			// No. Its not possible, non of their attackers line up with the square
			//attacker.setAsInvalid();	// No attacker was found
		}

		if (attacker == BoardSquare{ 'a', '8' }) {
			std::cout << "Bingooo" << attacker.isValid() << std::endl;
			std::cin.get();
		}

		return attacker;
	}

	template<typename KNIGHT_DIRECTION_T>
	BoardSquare Attackers::findAttackingKnight(BoardSquare attackedSquare, const Board & board, BitBoard theirs)
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
			// No. Its not possible, non of their attackers line up with the square
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