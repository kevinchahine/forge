#pragma once

#include "BitBoard.h"
#include "BoardSquare.h"
#include "Piece.h"

#include <iostream>

namespace forge
{
	const BitBoard pawn_mask = 0x00FFFFFFFFFFFF00ULL;

	class Board
	{
	public:
		
		void print(std::ostream & os = std::cout) const;

		Piece at(int row, int col) const;
		Piece at(BoardSquare square) const;

		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void setPiece(uint8_t row, uint8_t col, Piece piece) {	setPiece(BoardSquare( (uint16_t)row, (uint16_t)col ), piece); }
		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void setPiece(BoardSquare square, Piece piece);

		size_t rows() const { return 8; }
		size_t cols() const { return 8; }

		BitBoard occupied() const { return m_whites | m_blacks; }
		BitBoard whites() const { return m_whites; }
		BitBoard blacks() const { return m_blacks; }
		BitBoard pawns() const { return m_pawns & pawn_mask; }
		BitBoard en_passent() const { return m_pawns & ~pawn_mask; }
		BitBoard rooks() const { return m_rooks & ~m_bishops; }
		BitBoard bishops() const { return m_bishops & ~m_rooks; }
		BitBoard queens() const { return m_rooks & m_bishops; }
		BitBoard knights() const { 
			BitBoard knightsBB =
				occupied() &
				~pawns() &
				~m_bishops &
				~m_rooks;
			
			knightsBB[m_whiteKing] = 0;
			knightsBB[m_blackKing] = 0;

			return knightsBB;
		}
		BitBoard kings() const {
			BitBoard kingsBB;

			kingsBB[m_whiteKing] = 1;
			kingsBB[m_blackKing] = 1;

			return kingsBB;
		}

	private:
		BitBoard m_whites;			// All White Pieces (Including Kings and Knights)
		BitBoard m_blacks;			// All Black Pieces (Including Kings and Knights)
		BitBoard m_bishops;			// Bishops and Queens
		BitBoard m_rooks;			// Rooks and Queens
		// Ranks 1 and 8 have special meaning. Pawn at rank 1 means that
		// corresponding white pawn on rank 4 can be taken en passant. Rank 8 is the
		// same for black pawns. Those "fake" pawns are not present in our_pieces_ and
		// their_pieces_ bitboards.
		BitBoard m_pawns;			// Pawns (Ranks 1 and 8 represent which pawns can do enpassent)
		
		BoardSquare m_whiteKing;
		BoardSquare m_blackKing;

		// !!! Still need castiling
	};
} // namespace forge