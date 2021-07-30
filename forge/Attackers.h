#pragma once

#include "Board.h"
#include "BoardSquare.h"

namespace forge
{
	class Attackers
	{
	public:
		template<typename RAY_DIRECTION_T>
		static BoardSquare findAttackingRay(
			BoardSquare attackedSquare,
			const Board & board,
			BitBoard theirs,
			BitBoard ours);

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