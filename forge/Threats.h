#pragma once

#include "Attackers.h"
#include "Board.h"

namespace forge
{
	class Threats
	{
	public: // ------------------------ STATIC METHODS ------------------------

		static BitBoard genThreats(const Board & board, BitBoard attackers);

		static BitBoard genThreats(const Board & board, BoardSquare attacker, BitBoard obstacles);

		// !!!Excludes Kings as obstacles
		template<typename PIECE_T> static BitBoard genThreatsFor(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Bishop>(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Rook>(BoardSquare attacker, BitBoard obstacles);
		template<> static BitBoard genThreatsFor<pieces::Queen>(BoardSquare attacker, BitBoard obstacles);
	};
} // namespace forge

#include "ThreatsDefinitions.h"