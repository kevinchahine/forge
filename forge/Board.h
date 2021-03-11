#pragma once

#include "BitBoard.h"
#include "BoardSquare.h"
#include "Piece.h"
#include "Move.h"

#include <Guten/Color.h>
#include <Guten/Matrix.h>

#include <iostream>
#include <vector>
#include <tuple>

namespace forge
{
	const BitBoard pawn_mask = 0x00FFFFFFFFFFFF00ULL;

	class Board
	{
	public:
		// --- Declare Friend Classes ---
		class MoveGenerator;
		friend MoveGenerator;

		void print(std::ostream & os = std::cout) const;
		void printMini(std::ostream & os = std::cout) const;

		guten::core::Matrix getMiniBoard(
			const guten::color::Color & lightPiece = guten::color::white,
			const guten::color::Color & darkPiece = guten::color::black,
			const guten::color::Color & lightCell = guten::color::brown,
			const guten::color::Color & darkCell = guten::color::green) const;

		Piece at(int row, int col) const;
		Piece at(BoardSquare square) const;

		// Does not remove Kings
		// !!! Do not use to remove Kings !!!
		// Only call on cells that are guarenteed to have a piece on them (other than kings).
		// !!! Calling on a cell that is already empty or is occupied by a king
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

		// Places a piece at square
		// Only call on empty cells
		// Do not call on a cell that is occupied by a piece
		// !!! Does not account captures or special moves (castling, promotions, enpassent) !!!
		void placeWhitePawn(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBlackPawn(BoardSquare square);
		// See comment for placeWhitePawn()
		void placePawn(BoardSquare square, bool isWhite) 
		{ 
			isWhite ? placeWhitePawn(square) : placeBlackPawn(square); 
		}
		
		// See comment for placeWhitePawn()
		void placeWhiteRook(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBlackRook(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeRook(BoardSquare square, bool isWhite)
		{
			isWhite ? placeWhiteRook(square) : placeBlackRook(square);
		}

		// See comment for placeWhitePawn()
		void placeWhiteKnight(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBlackKnight(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeKnight(BoardSquare square, bool isWhite)
		{
			isWhite ? placeWhiteKnight(square) : placeBlackKnight(square);
		}

		// See comment for placeWhitePawn()
		void placeWhiteBishop(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBlackBishop(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBishop(BoardSquare square, bool isWhite)
		{
			isWhite ? placeWhiteBishop(square) : placeBlackBishop(square);
		}

		// See comment for placeWhitePawn()
		void placeWhiteQueen(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeBlackQueen(BoardSquare square);
		// See comment for placeWhitePawn()
		void placeQueen(BoardSquare square, bool isWhite)
		{
			isWhite ? placeWhiteQueen(square) : placeBlackQueen(square);
		}

		// Moves White King from its current position to square
		// Make sure that square refers to an empty square.
		// !!! Moving king to an occupied square can have unexpected results
		// !!! Does not account for captures or castling !!!
		void moveWhiteKing(BoardSquare square);
		// See comment for moveWhiteKing()
		void moveBlackKing(BoardSquare square);
		// See comment for moveWhiteKing()
		void moveKing(BoardSquare square, bool isWhite);
		// Make sure that from is the coordinate of the white or black king
		// !!!Make sure that to is empty before calling this method!!!
		// See comment for moveWhiteKing()
		void moveKing(BoardSquare from, BoardSquare to);

		void movePiece(BoardSquare from, BoardSquare to);

		bool isPawn(BoardSquare square) const { return pawns()[square] == 1; }
		bool isRook(BoardSquare square) const { return rooks()[square] == 1; }
		bool isKnight(BoardSquare square) const { return knights()[square] == 1; }
		bool isBishop(BoardSquare square) const { return bishops()[square] == 1; }
		bool isQueen(BoardSquare square) const { return queens()[square] == 1; }
		bool isKing(BoardSquare square) const { return kings()[square] == 1; }

		// Removes all pieces except Kings.
		// Places Kings in there starting locations.
		void reset();

		// Places all pieces as they would go at the start of a normal chess game.
		void placeAllPieces();

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
		BitBoard m_whites{ uint64_t(1) << 60 };	// All White Pieces (Including Kings and Knights)
		BitBoard m_blacks{ uint64_t(1) << 4 };	// All Black Pieces (Including Kings and Knights)
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
	

} // namespace forge