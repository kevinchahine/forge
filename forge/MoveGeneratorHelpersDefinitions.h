#pragma once

#include "MoveGeneratorHelpers.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenHelpers::findAttacker(BoardSquare attackedSquare, const Board & board, BitBoard theirs, BitBoard ours)
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

	template<typename PIECE_T>
	BitBoard MoveGenHelpers::genThreatsFor(BoardSquare attacker, BitBoard obstacles)
	{
		static_assert(std::is_base_of<pieces::Piece, PIECE_T>(),
			"PIECE_T needs to be a type deriving from pieces::Piece");

		// This template specialization works for King, Knight, WhitePawn and BlackPawn
		return PIECE_T::captureMask(attacker);
	}

	template<> BitBoard MoveGenHelpers::genThreatsFor<pieces::Rook>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;
		
		threats |= MoveGenHelpers::genAttackRay<directions::Up>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::Down>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::Left>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::Right>(attacker, obstacles);
	
		return threats;
	}

	template<> BitBoard MoveGenHelpers::genThreatsFor<pieces::Bishop>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;
	
		threats |= MoveGenHelpers::genAttackRay<directions::UL>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::UR>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::DR>(attacker, obstacles);
		threats |= MoveGenHelpers::genAttackRay<directions::DL>(attacker, obstacles);
	
		return threats;
	}
	
	template<> BitBoard MoveGenHelpers::genThreatsFor<pieces::Queen>(BoardSquare attacker, BitBoard obstacles)
	{
	return
		MoveGenHelpers::genThreatsFor<pieces::Bishop>(attacker, obstacles) |
		MoveGenHelpers::genThreatsFor<pieces::Rook>(attacker, obstacles);
	}
	
	template<typename RAY_DIRECTION_T>
	BitBoard MoveGenHelpers::genAttackRay(BoardSquare attacker, BitBoard obstacles)
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