#pragma once

#include "BitBoard.h"
#include "BoardSquare.h"
#include "Piece.h"
#include "Move.h"

#include <iostream>
#include <vector>
#include <tuple>

namespace forge
{
	const BitBoard pawn_mask = 0x00FFFFFFFFFFFF00ULL;

	class Board
	{
	public:

		void print(std::ostream & os = std::cout) const;
		void printMini(std::ostream & os = std::cout) const;

		Piece at(int row, int col) const;
		Piece at(BoardSquare square) const;

		// Does not remove Kings
		// Only call on cells that are guarenteed to have a piece on them (other than kings).
		// Calling on a cell that is already empty or is occupied by a king
		//	may have unexpected results
		void removePiece(BoardSquare pos);
		
		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void placePiece(uint8_t row, uint8_t col, Piece piece) {	placePiece(BoardSquare( (uint16_t)row, (uint16_t)col ), piece); }
		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void placePiece(BoardSquare square, Piece piece);

		// Places a piece at pos
		// Only call on empty cells
		// Do not call on a cell that is occupied by a piece
		// !!! Does not account captures or special moves (castling, promotions, enpassent) !!!
		void placeWhitePawn(BoardSquare pos);
		// See comment for placeWhitePawn()
		void placeBlackPawn(BoardSquare pos);
		
		// See comment for placeWhitePawn()
		void placeWhiteRook(BoardSquare pos);
		// See comment for placeWhitePawn()
		void placeBlackRook(BoardSquare pos);
		
		// See comment for placeWhitePawn()
		void placeWhiteKnight(BoardSquare pos);
		// See comment for placeWhitePawn()
		void placeBlackKnight(BoardSquare pos);
		
		// See comment for placeWhitePawn()
		void placeWhiteBishop(BoardSquare pos);
		// See comment for placeWhitePawn()
		void placeBlackBishop(BoardSquare pos);
		
		// See comment for placeWhitePawn()
		void placeWhiteQueen(BoardSquare pos);
		// See comment for placeWhitePawn()
		void placeBlackQueen(BoardSquare pos);

		// Moves White King from its current position to pos
		// Make sure that pos refers to an empty square.
		// Moving king to an occupied square can have unexpected results
		// !!! Does not account captures or or castling !!!
		void moveWhiteKing(BoardSquare pos);
		// See comment for moveWhiteKing()
		void moveBlackKing(BoardSquare pos);

		bool isPawn(BoardSquare pos) const { return pawns()[pos] == 1; }
		bool isRook(BoardSquare pos) const { return rooks()[pos] == 1; }
		bool isKnight(BoardSquare pos) const { return knights()[pos] == 1; }
		bool isBishop(BoardSquare pos) const { return bishops()[pos] == 1; }
		bool isQueen(BoardSquare pos) const { return queens()[pos] == 1; }
		bool isKing(BoardSquare pos) const { return kings()[pos] == 1; }

		// Removes all pieces except Kings.
		// Places Kings in there starting locations.
		void reset();

		// Places all pieces as they would go at the start of a normal chess game.
		void placeAllPieces();

		std::vector<std::pair<Move, Board>> generatePsuedoLegalMovesWhite() const;
		std::vector<std::pair<Move, Board>> generatePsuedoLegalMovesBlack() const;

		size_t rows() const { return 8; }
		size_t cols() const { return 8; }

		BitBoard occupied() const { return m_whites | m_blacks; }
		BitBoard empty() const { return ~occupied(); }
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
		BitBoard m_whites{ 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000 };	// All White Pieces (Including Kings and Knights)
		BitBoard m_blacks{ 1u << 4 };	// All Black Pieces (Including Kings and Knights)
		BitBoard m_bishops;				// Bishops and Queens
		BitBoard m_rooks;				// Rooks and Queens
		// Ranks 1 and 8 have special meaning. Pawn at rank 1 means that
		// corresponding white pawn on rank 4 can be taken en passant. Rank 8 is the
		// same for black pawns. Those "fake" pawns are not present in our_pieces_ and
		// their_pieces_ bitboards.
		BitBoard m_pawns;			// Pawns (Ranks 1 and 8 represent which pawns can do enpassent)
		
		BoardSquare m_whiteKing{ 60 };
		BoardSquare m_blackKing{ 4 };

		// !!! Still need castiling
	};
	
	using MoveList = std::vector<std::pair<Move, Board>>;

} // namespace forge