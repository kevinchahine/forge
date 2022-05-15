#pragma once

#include "Color.h"
#include "BitBoard.h"
#include "BoardSquare.h"
#include "Piece.h"
#include "Move.h"

#include <Guten/color/Color.h>
#include <Guten/boards/CheckerBoard.h>

#include <iostream>
#include <vector>
#include <tuple>
#include <assert.h>	// for assert() // TODO: dO WE STILL need this

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
		friend struct std::hash<Board>;

		// Removes allToFen pieces except Kings.
		// Places Kings in there starting locations.
		void reset();

		constexpr size_t rows() const { return 8; }
		constexpr size_t cols() const { return 8; }

		void print(std::ostream & os = std::cout) const;
		void printMini(std::ostream & os = std::cout) const;

		guten::boards::CheckerBoard getCheckerBoard() const;
		guten::core::Matrix getMiniBoard(
			const termcolor::Color & lightPiece = guten::color::white,
			const termcolor::Color & darkPiece = guten::color::black,
			const termcolor::Color & lightCell = guten::color::darkyellow,
			const termcolor::Color & darkCell = guten::color::green) const;

		pieces::Piece at(int row, int col) const;
		pieces::Piece at(BoardSquare square) const;

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

		// Places allToFen pieces as they would go at the start of a normal chess game.
		void placeAllPieces();

		template <typename PIECE_T>	void place(BoardSquare square, bool isWhite = true);
		/////////////template<> void place<pieces::Empty>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::King>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::WhiteKing>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::BlackKing>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::Queen>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::Bishop>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::Knight>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::Rook>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::Pawn>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::WhitePawn>(BoardSquare square, bool isWhite);
		/////////////template<> void place<pieces::BlackPawn>(BoardSquare square, bool isWhite);

		// Works for both push moves and captures
		template <typename PIECE_T> void move(Move move);
		//////////////template<> void move<pieces::King>(Move move);
		//////////////template<> void move<pieces::WhiteKing>(Move move);
		//////////////template<> void move<pieces::BlackKing>(Move move);
		//////////////template<> void move<pieces::QBN_Piece>(Move move);
		//////////////template<> void move<pieces::Queen>(Move move);
		//////////////template<> void move<pieces::Bishop>(Move move);
		//////////////template<> void move<pieces::Knight>(Move move);
		//////////////template<> void move<pieces::Rook>(Move move);
		//////////////template<> void move<pieces::WhitePawn>(Move move);
		//////////////template<> void move<pieces::BlackPawn>(Move move);
		//////////////template<> void move<pieces::Pawn>(Move move);
		//////////////template<> void move<pieces::Piece>(Move move);

		// Rotates the Board 180 degrees. 
		// Same as rotating pieces but without rotating the chess board.
		// Does flip piece colors (white->black, black->white)
		Board rotated() const;

		// ---------------------------- Boolean Methods -----------------------

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
		
		// ---------------------------- BitBoard Piece Accessors --------------

		template<typename COLOR_T> BitBoard colors() const;
		
		BitBoard occupied() const { return m_whites | m_blacks; }
		BitBoard empty() const { return ~occupied(); }
		BitBoard whites() const { return m_whites; }
		BitBoard blacks() const { return m_blacks; }
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

		BoardSquare whiteKing() const { return m_whiteKing; }
		BoardSquare blackKing() const { return m_blackKing; }

		// Returns the bitboard for the specified piece type
		// Piece type can come from class forge::pieces::Piece and be of type
		// forge::pieces::Piece::piece_t.
		template<typename PIECE_T>
		BitBoard pieces() const;
		//{
		//	std::cout << "Don't use this method. Use full specialization overloads instead. PIECE_T = " << typeid(PIECE_T).name() << "\n";
		//	assert(false);
		//	///static_assert(false, "Don't use this method. Use full specialization overloads instead\n");
		//	static_assert(false);
		//
		//	return BitBoard();
		//}
		
		template<typename DIRECTION_T> BitBoard directionals() const;
		
		bool operator==(const Board & rhs) const {
			return
				(m_whites == rhs.m_whites) &&
				(m_blacks == rhs.m_blacks) &&
				(m_bishops == rhs.m_bishops) &&
				(m_rooks == rhs.m_rooks) &&
				(m_pawns == rhs.m_pawns) &&				// includes enpassent bits
				(m_whiteKing == rhs.m_whiteKing) &&
				(m_blackKing == rhs.m_blackKing);
			// TODO: Castling: Don't forget Castling
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
		BitBoard m_pawns;			// Pawns (Ranks 1 and 8 represent which pawns have just moved 2 squares)

		BoardSquare m_whiteKing{ 60 };
		BoardSquare m_blackKing{ 4 };

		// TODO: Still need castiling
	}; // class Board

	// -------------------------------- EXPLICIT SPECIALIZATIONS --------------

} // namespace forge

// --- Inject hash into std namespace
namespace std
{
	template<> struct hash<forge::Board>
	{
	public:
		std::size_t operator()(const forge::Board& b) const noexcept;
	};
}