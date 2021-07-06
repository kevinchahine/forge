#pragma once

#include "Board.h"
#include "BoardSquare.h"
#include "AttackerPair.h"

namespace forge
{
	class MoveGenHelpers 
	{
	public:
		template<typename RAY_DIRECTION_T>
		static BoardSquare findAttacker(BoardSquare attackedSquare, const Board & board, BitBoard theirs, BitBoard ours);

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

		// Searches for any opponent pieces that might be attacking 'ourKing'
		// Returns up to 2 BoardSquares that locate what pieces are attacking 'ourKing'
		// Calling this function should be avoided for performance if 'ourKing' stands on a safe square.
		// This can be determined using the threat board.
		static AttackerPair findKingAttackers(const Board & board, BoardSquare ourKing, BitBoard theirs, BitBoard ours);
	}; // class MoveGenHelpers
} // namespace forge

#include "MoveGeneratorHelpersDefinitions.h"