#include "Threats.h"

using namespace std;

namespace forge
{
	BitBoard Threats::genThreats(const Board & board, BitBoard attackers)
	{
		BitBoard threats;

		// obstacles is made up of all pieces (including their king) excluding our king
		BitBoard obstacles = board.occupied() & ~(board.kings() & ~attackers);

		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare attacker{ row, col };

				if (attackers[attacker]) {
					threats |= genThreats(board, attacker, obstacles);
				}
			}
		}

		return threats;
	}

	BitBoard Threats::genThreats(const Board & board, BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		// Placed in order of most common to least common pieces
		if (board.isPawn(attacker)) {
			if (board.isWhite(attacker)) threats = genThreatsFor<pieces::WhitePawn>(attacker, obstacles);
			else threats = genThreatsFor<pieces::BlackPawn>(attacker, obstacles);
		}
		else if (board.isKnight(attacker)) threats = genThreatsFor<pieces::Knight>(attacker, obstacles);
		else if (board.isBishop(attacker)) threats = genThreatsFor<pieces::Bishop>(attacker, obstacles);
		else if (board.isRook(attacker)) threats = genThreatsFor<pieces::Rook>(attacker, obstacles);
		else if (board.isQueen(attacker)) threats = genThreatsFor<pieces::Queen>(attacker, obstacles);
		else if (board.isKing(attacker)) threats = genThreatsFor<pieces::King>(attacker, obstacles);
#ifdef _DEBUG
		else {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << "\n\t"
				<< "Piece could not be identified.  Is this square even occupied\n";
		}
#endif // _DEBUG

		return threats;
	}

	template<> BitBoard Threats::genThreatsFor<pieces::Rook>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		threats |= Attackers::genAttackLineSegment<directions::Up>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::Down>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::Left>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::Right>(attacker, obstacles);

		return threats;
	}

	template<> BitBoard Threats::genThreatsFor<pieces::Bishop>(BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		threats |= Attackers::genAttackLineSegment<directions::UL>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::UR>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::DR>(attacker, obstacles);
		threats |= Attackers::genAttackLineSegment<directions::DL>(attacker, obstacles);

		return threats;
	}

	template<> BitBoard Threats::genThreatsFor<pieces::Queen>(BoardSquare attacker, BitBoard obstacles)
	{
		return
			Threats::genThreatsFor<pieces::Bishop>(attacker, obstacles) |
			Threats::genThreatsFor<pieces::Rook>(attacker, obstacles);
	}
} // namespace forge