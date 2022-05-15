#pragma once

#include "Board.h"
#include "BoardSquare.h"

namespace forge
{
	class Attackers
	{
	public:
		// Searches in a specified direction for a piece that is attacking a square
		// square - Coordinate of the square we are interested in.
		// board - The board with all its pieces. 
		// aggressors - ALL pieces that are of the same color as the attacker we are looking for.
		// return - If attacker exists: coordinate of the attacker 
		//			Else an invalid BoardSquare
		template<typename RAY_DIRECTION_T>
		static BoardSquare findAttackingRay(
			BoardSquare square,
			const Board & board, 
			BitBoard aggressors);

		template <typename KNIGHT_DIRECTION_T>
		static BoardSquare findAttackingKnight(BoardSquare attackedSquare, const Board & board, BitBoard theirs);

		// Creates a BitBoard of allToFen the cells that some attacker can reach until it hits an obstacle
		// This method treats both pieces of colors as obstacles. 
		// This way an attacker can "attack" its opponents as well as its own pieces.
		template<typename RAY_DIRECTION_T> 
		static BitBoard genAttackLineSegment(BoardSquare attacker, BitBoard obstacles);
	};
} // namespace forge

#include "AttackersDefinitions.h"