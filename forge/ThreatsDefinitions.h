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

	template<> BitBoard Threats::genThreatsFor<pieces::Rook>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		threats |= Attackers::genAttackRay<directions::Up>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::Down>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::Left>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::Right>(attacker, obstacles);

		return threats;
	}

	template<> BitBoard Threats::genThreatsFor<pieces::Bishop>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		threats |= Attackers::genAttackRay<directions::UL>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::UR>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::DR>(attacker, obstacles);
		threats |= Attackers::genAttackRay<directions::DL>(attacker, obstacles);

		return threats;
	}

	template<> BitBoard Threats::genThreatsFor<pieces::Queen>(BoardSquare attacker, BitBoard obstacles)
	{
		return
			Threats::genThreatsFor<pieces::Bishop>(attacker, obstacles) |
			Threats::genThreatsFor<pieces::Rook>(attacker, obstacles);
	}

} // namespace forge