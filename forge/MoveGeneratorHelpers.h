#pragma once

#include "Board.h"
#include "BoardSquare.h"
#include "KingAttackers.h"

namespace forge
{
	class MoveGenHelpers 
	{
	public:
		
		static BitBoard genThreats(const Board & board, BitBoard attackers);

		static BitBoard genThreats(const Board & board, BoardSquare attacker, BitBoard obstacles);
		
		// !!!Excludes Kings as obstacles
		template<typename PIECE_T> static BitBoard genThreatsFor(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Bishop>(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Rook>(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Queen>(BoardSquare attacker, BitBoard obstacles);

		// Creates a BitBoard of all the cells that some attacker can reach until it hits an obstacle
		// This method treats both colors as obstacles. 
		// This way an attacker can "attack" its own pieces as well as opponents.
		template<typename RAY_DIRECTION_T> static BitBoard genAttackRay(BoardSquare attacker, BitBoard obstacles);

	}; // class MoveGenHelpers
} // namespace forge

#include "MoveGeneratorHelpersDefinitions.h"