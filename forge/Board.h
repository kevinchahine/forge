#pragma once

#include "BitBoard.h"
#include "BoardSquare.h"

namespace forge
{
	class Board
	{
	public:

	private:
		BitBoard m_whitesPices;		// All White Pieces
		BitBoard m_blacksPieces;	// All Black Pieces
		BitBoard m_bishops;			// Bishops and Queens
		BitBoard m_rooks;			// Rooks and Queens
		// Ranks 1 and 8 have special meaning. Pawn at rank 1 means that
		// corresponding white pawn on rank 4 can be taken en passant. Rank 8 is the
		// same for black pawns. Those "fake" pawns are not present in our_pieces_ and
		// their_pieces_ bitboards.
		BitBoard m_pawns;			// Pawns (Ranks 1 and 8 represent which pawns can do enpassent)
		
		BoardSquare m_whiteKing;
		BoardSquare m_blackKing;
	};
} // namespace forge