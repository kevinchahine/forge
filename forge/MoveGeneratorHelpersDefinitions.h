#pragma once

#include "MoveGeneratorHelpers.h"

namespace forge
{
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
	
} // namespace forge