#pragma once

#include "Threats.h"

namespace forge
{
	template<typename PIECE_T>
	BitBoard Threats::genThreatsFor(BoardSquare attacker, BitBoard obstacles)
	{
		static_assert(std::is_base_of<pieces::Piece, PIECE_T>(),
			"PIECE_T needs to be a type deriving from pieces::Piece");

		// This template specialization works for King, Knight, WhitePawn and BlackPawn
		return PIECE_T::captureMask(attacker);
	}
} // namespace forge