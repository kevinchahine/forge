#include "MoveGeneratorHelpers.h"

using namespace std;

namespace forge
{
	BitBoard MoveGenHelpers::genThreats(const Board & board, BitBoard attackers)
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

	BitBoard MoveGenHelpers::genThreats(const Board & board, BoardSquare attacker, BitBoard obstacles)
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
} // namespace forge

