#pragma once

#include "Color.h"
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
		friend class MoveGenerator;
		friend class GameState;
		friend class AttackChecker;

		void print(std::ostream & os = std::cout) const;
		void printMini(std::ostream & os = std::cout) const;

		guten::core::Matrix getMiniBoard(
			const guten::color::Color & lightPiece = guten::color::white,
			const guten::color::Color & darkPiece = guten::color::black,
			const guten::color::Color & lightCell = guten::color::brown,
			const guten::color::Color & darkCell = guten::color::green) const;

		pieces::Piece at(int row, int col) const;
		pieces::Piece at(BoardSquare square) const;

		// Does not remove Kings
		// Only call on cells that are guarenteed to have a piece on them (other than kings).
		// !!! Calling on a cell that is already empty or is occupied by a king
		//	may have unexpected results
		void removePiece(BoardSquare pos);
		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void placePiece(uint8_t row, uint8_t col, pieces::Piece piece) { placePiece(BoardSquare((uint16_t)row, (uint16_t)col), piece); }
		// If piece == empty, does not remove King.
		// To move King, simply set the desired coordinates using this method.
		// Optimization: Not intended to be used in performance critical code.
		// Use methods that move pieces instead
		void placePiece(BoardSquare square, pieces::Piece piece);
		// Places 0 or more pieces of the same type.
		// Not an efficient method.
		// Does not work on Kings.
		// * See comments for void Board::placePiece(BoardSquare square, pieces::Piece piece)
		void placePieces(BitBoard squares, pieces::Piece piece);

		template <typename PIECE_T>	void place(BoardSquare square, bool isWhite);
		template<> void place<pieces::Empty>(BoardSquare square, bool isWhite);
		template<> void place<pieces::Queen>(BoardSquare square, bool isWhite);
		template<> void place<pieces::Bishop>(BoardSquare square, bool isWhite);
		template<> void place<pieces::Knight>(BoardSquare square, bool isWhite);
		template<> void place<pieces::Rook>(BoardSquare square, bool isWhite);
		template<> void place<pieces::WhitePawn>(BoardSquare square, bool isWhite);
		template<> void place<pieces::BlackPawn>(BoardSquare square, bool isWhite);
		///template<> void place<pieces::King>(BoardSquare square, bool isWhite);

		template <typename PIECE_T> void move(Move move);
		template<> void move<pieces::Empty>(Move move);
		template<> void move<pieces::Queen>(Move move);
		template<> void move<pieces::Bishop>(Move move);
		template<> void move<pieces::Knight>(Move move);
		template<> void move<pieces::Rook>(Move move);
		template<> void move<pieces::WhitePawn>(Move move);
		template<> void move<pieces::BlackPawn>(Move move);
		template<> void move<pieces::King>(Move move);

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

		bool isOccupied(BoardSquare square) const { return occupied()[square]; }
		bool isEmpty(BoardSquare square) const { return empty()[square]; }
		bool isKing(BoardSquare square) const { return kings()[square]; }
		bool isQueen(BoardSquare square) const { return queens()[square]; }
		bool isBishop(BoardSquare square) const { return bishops()[square]; }
		bool isKnight(BoardSquare square) const { return knights()[square]; }
		bool isRook(BoardSquare square) const { return rooks()[square]; }
		bool isPawn(BoardSquare square) const { return pawns()[square]; }
		bool isWhite(BoardSquare square) const { return whites()[square]; }
		bool isBlack(BoardSquare square) const { return blacks()[square]; }

		template <typename PIECE_T> bool isPiece(BoardSquare square) const;
		template<> bool isPiece<pieces::King>(BoardSquare square) const { return isKing(square); }
		template<> bool isPiece<pieces::Queen>(BoardSquare square) const { return isQueen(square); }
		template<> bool isPiece<pieces::Bishop>(BoardSquare square) const { return isBishop(square); }
		template<> bool isPiece<pieces::Knight>(BoardSquare square) const { return isKnight(square); }
		template<> bool isPiece<pieces::Rook>(BoardSquare square) const { return isRook(square); }
		template<> bool isPiece<pieces::WhitePawn>(BoardSquare square) const { return isWhite(square) && isPawn(square); }
		template<> bool isPiece<pieces::BlackPawn>(BoardSquare square) const { return isBlack(square) && isPawn(square); }
		template<> bool isPiece<colors::White>(BoardSquare square) const { return isWhite(square); }
		template<> bool isPiece<colors::Black>(BoardSquare square) const { return isBlack(square); }

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
		template<typename COLOR_T> BitBoard colors() const;
		template<> BitBoard colors<colors::White>() const { return whites(); }
		template<> BitBoard colors<colors::Black>() const { return blacks(); }
		BitBoard pawns() const { return m_pawns & pawn_mask; }
		BitBoard en_passent() const { return m_pawns & ~pawn_mask; }
		BitBoard rooks() const { return m_rooks & ~m_bishops; }
		BitBoard bishops() const { return m_bishops & ~m_rooks; }
		BitBoard queens() const { return m_rooks & m_bishops; }
		BitBoard laterals() const { return m_rooks; }
		BitBoard diagonals() const { return m_bishops; }
		BitBoard rays() const { return m_rooks | m_bishops; }
		BitBoard blockers() const { return occupied() & ~kings(); }
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

		template<typename DIRECTION_T> BitBoard directionals() const;
		template<> BitBoard directionals<directions::Linear>() const { return laterals() & diagonals(); }
		template<> BitBoard directionals<directions::Lateral>() const { return laterals(); }
		template<> BitBoard directionals<directions::Diagonal>() const { return diagonals(); }
		template<> BitBoard directionals<directions::LShape>() const { return knights(); }
		template<> BitBoard directionals<directions::Ray>() const { return rays(); }
		template<> BitBoard directionals<directions::Up>() const { return rays(); }
		template<> BitBoard directionals<directions::Down>() const { return rays(); }
		template<> BitBoard directionals<directions::Left>() const { return rays(); }
		template<> BitBoard directionals<directions::Right>() const { return rays(); }
		template<> BitBoard directionals<directions::UR>() const { return rays(); }
		template<> BitBoard directionals<directions::UL>() const { return rays(); }
		template<> BitBoard directionals<directions::DL>() const { return rays(); }
		template<> BitBoard directionals<directions::DR>() const { return rays(); }

		BoardSquare whiteKing() const { return m_whiteKing; }
		BoardSquare blackKing() const { return m_blackKing; }

		// Returns the bitboard for the specified piece type
		// Piece type can come from class forge::pieces::Piece and be of type
		// forge::pieces::Piece::piece_t.
		template<pieces::Piece::piece_t>
		BitBoard pieces() const
		{
			static_assert(false, "Don't use this method. Use full specialization overloads instead\n");

			return BitBoard();
		}
		template<> BitBoard pieces<pieces::Piece::KING>() const { return kings(); }
		template<> BitBoard pieces<pieces::Piece::QUEEN>() const { return queens(); }
		template<> BitBoard pieces<pieces::Piece::BISHOP>() const { return bishops(); }
		template<> BitBoard pieces<pieces::Piece::KNIGHT>() const { return knights(); }
		template<> BitBoard pieces<pieces::Piece::ROOK>() const { return rooks(); }
		template<> BitBoard pieces<pieces::Piece::PAWN>() const { return pawns(); }

		bool operator==(const Board & rhs) const {
			return
				(m_whites == rhs.m_whites) &&
				(m_blacks == rhs.m_blacks) &&
				(m_bishops == rhs.m_bishops) &&
				(m_rooks == rhs.m_rooks) &&
				(m_pawns == rhs.m_pawns) &&				// includes enpassent bits
				(m_whiteKing == rhs.m_whiteKing) &&
				(m_blackKing == rhs.m_blackKing);
			// TODO: Don't forget Castling
		}
		bool operator!=(const Board & rhs) const { return !(*this == rhs); }

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

		// TODO: Still need castiling
	};
} // namespace forge

#include "Board.h"