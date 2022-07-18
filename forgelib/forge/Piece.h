#pragma once

#include "forge/BitBoard.h"
#include "forge/Color.h"

#include <iostream>
#include <bitset>

////#include <Guten/color/Color.h>
////#include <Guten/termcolor/termcolor.hpp>

namespace forge
{
	// Data type alias to represent piece color
	using color_t = bool;
	const color_t WHITE = 1;
	const color_t BLACK = 0;

	namespace pieces {
		class Piece
		{
		public:
			using piece_t = uint8_t;

			static const piece_t EMPTY = 0b0000;
			static const piece_t EMPTY_FLIPPED = 0b1000;

			static const piece_t WHITE_KING   = 0b0001;
			static const piece_t BLACK_KING   = 0b1001;
			static const piece_t WHITE_QUEEN  = 0b0010;
			static const piece_t BLACK_QUEEN  = 0b1010;
			static const piece_t WHITE_BISHOP = 0b0011;
			static const piece_t BLACK_BISHOP = 0b1011;
			static const piece_t WHITE_KNIGHT = 0b0100;
			static const piece_t BLACK_KNIGHT = 0b1100;
			static const piece_t WHITE_ROOK   = 0b0101;
			static const piece_t BLACK_ROOK   = 0b1101;
			static const piece_t WHITE_PAWN   = 0b0110;
			static const piece_t BLACK_PAWN   = 0b1110;

			static const piece_t KING = WHITE_KING;
			static const piece_t QUEEN = WHITE_QUEEN;
			static const piece_t BISHOP = WHITE_BISHOP;
			static const piece_t KNIGHT = WHITE_KNIGHT;
			static const piece_t ROOK = WHITE_ROOK;
			static const piece_t PAWN = WHITE_PAWN;

		public:
			Piece() : m_val(EMPTY) {}
			Piece(piece_t value) : m_val(value) {}
			Piece(char ch, bool isWhite = true) { setCh(ch, isWhite); }
			Piece(const Piece &) = default;
			Piece(Piece &&) noexcept = default;
			virtual ~Piece() noexcept = default;
			Piece & operator=(const Piece &) = default;
			Piece & operator=(Piece &&) noexcept = default;

			bool operator==(const Piece & piece) const { return this->m_val == piece.m_val; }
			bool operator!=(const Piece & piece) const { return this->m_val != piece.m_val; }

			friend std::ostream & operator<<(std::ostream & os, const Piece & p)
			{
				os << termcolor::push
					<< (p.isBlack() ? guten::color::yellow : guten::color::green)
					<< p.getCh()
					<< termcolor::pop;

				return os;
			}
			friend std::istream & operator>>(std::istream & is, Piece & p) {
				char ch;

				is >> ch;

				p.setCh(ch);

				return is;
			}

			std::bitset<8> val() const { return m_val; }
			std::bitset<8> & val() { return m_val; }

			char getCh() const;
			colors::Color getColor() const;

			// Set piece based on character
			// Piece color is set to white by default
			void setCh(char ch, bool isWhite = true);

			bool isWhite() const { return m_val[3] == 0; }
			bool isBlack() const { return !isWhite(); }
			bool isEmpty() const { return m_val == EMPTY || m_val == EMPTY_FLIPPED; }
			bool isOccupied() const { return !isEmpty(); }
			bool isKing() const { return (m_val == WHITE_KING) || (m_val == BLACK_KING); }
			bool isQueen() const { return (m_val == WHITE_QUEEN) || (m_val == BLACK_QUEEN); }
			bool isRook() const { return (m_val == WHITE_ROOK) || (m_val == BLACK_ROOK); }
			bool isBishop() const { return (m_val == WHITE_BISHOP) || (m_val == BLACK_BISHOP); }
			bool isKnight() const { return (m_val == WHITE_KNIGHT) || (m_val == BLACK_KNIGHT); }
			bool isPawn() const { return (m_val == WHITE_PAWN) || (m_val == BLACK_PAWN); }
			bool isRay() const { return isQueen() || isRook() || isBishop(); }

			// Flips color of piece:
			// black -> white
			// white -> black
			void flipColor() { m_val.flip(3); }
			void makeWhite() { m_val[3] = 0; }
			void makeBlack() { m_val[3] = 1; }

			BitBoard pushMask(BoardSquare square) const;

			BitBoard captureMask(BoardSquare square) const;

			void masks(BoardSquare square, BitBoard & pushMask, BitBoard & captureMask) const;

		protected:
			std::bitset<8> m_val = 0;
		};

		// QBR
		class RayPiece { };

		class QBN_Piece {};

		// QBNRP (Not K or Empty)
		class NonKingPiece { };

		const Piece empty{ Piece::EMPTY };
		const Piece whiteKing{ Piece::WHITE_KING };
		const Piece whiteQueen{ Piece::WHITE_QUEEN };
		const Piece whiteBishop{ Piece::WHITE_BISHOP };
		const Piece whiteKnight{ Piece::WHITE_KNIGHT };
		const Piece whiteRook{ Piece::WHITE_ROOK };
		const Piece whitePawn{ Piece::WHITE_PAWN };
		const Piece blackKing{ Piece::BLACK_KING };
		const Piece blackQueen{ Piece::BLACK_QUEEN };
		const Piece blackBishop{ Piece::BLACK_BISHOP };
		const Piece blackKnight{ Piece::BLACK_KNIGHT };
		const Piece blackRook{ Piece::BLACK_ROOK };
		const Piece blackPawn{ Piece::BLACK_PAWN };

		class Empty : public Piece {
		public:
			Empty() : Piece{ Piece::EMPTY } {}
			Empty(const Empty &) = default;
			Empty(Empty &&) noexcept = default;
			virtual ~Empty() noexcept = default;
			Empty & operator=(const Empty &) = default;
			Empty & operator=(Empty &&) noexcept = default;

			// Returns BitBoard containing allToFen zeros
			static BitBoard pushMask(BoardSquare square) { return BitBoard(); }

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};

		class King : public Piece {
		public:
			King() : Piece{ Piece::KING } {}
			King(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_KING : Piece::BLACK_KING) } {}
			King(King &&) noexcept = default;
			virtual ~King() noexcept = default;
			King & operator=(const King &) = default;
			King & operator=(King &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};

		class WhiteKing : public King {
		public:
			WhiteKing() : King{ forge::WHITE } {}
			WhiteKing(bool isWhite) : King{ isWhite } {}
			WhiteKing(WhiteKing &&) noexcept = default;
			virtual ~WhiteKing() noexcept = default;
			WhiteKing & operator=(const WhiteKing &) = default;
			WhiteKing & operator=(WhiteKing &&) noexcept = default;
		};

		class BlackKing : public King {
		public:
			BlackKing() : King{ forge::BLACK } {}
			BlackKing(bool isWhite) : King{ isWhite } {}
			BlackKing(BlackKing &&) noexcept = default;
			virtual ~BlackKing() noexcept = default;
			BlackKing & operator=(const BlackKing &) = default;
			BlackKing & operator=(BlackKing &&) noexcept = default;
		};

		class Queen : public Piece, public RayPiece, public NonKingPiece, public QBN_Piece {
		public:
			Queen() : Piece{ Piece::QUEEN } {}
			Queen(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_QUEEN : Piece::BLACK_QUEEN) } {}
			Queen(Piece::piece_t val) : Piece{ val } {}
			Queen(const Queen &) = default;
			Queen(Queen &&) noexcept = default;
			virtual ~Queen() noexcept = default;
			Queen & operator=(const Queen &) = default;
			Queen & operator=(Queen &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};

		class WhiteQueen : public Queen {
			WhiteQueen() : Queen{ Piece::WHITE_QUEEN } {}
			WhiteQueen(const WhiteQueen &) = default;
			WhiteQueen(WhiteQueen &&) noexcept = default;
			virtual ~WhiteQueen() noexcept = default;
			WhiteQueen & operator=(const WhiteQueen &) = default;
			WhiteQueen & operator=(WhiteQueen &&) noexcept = default;
		};

		class BlackQueen : public Queen {
			BlackQueen() : Queen{ Piece::BLACK_QUEEN } {}
			BlackQueen(const BlackQueen &) = default;
			BlackQueen(BlackQueen &&) noexcept = default;
			virtual ~BlackQueen() noexcept = default;
			BlackQueen & operator=(const BlackQueen &) = default;
			BlackQueen & operator=(BlackQueen &&) noexcept = default;
		};

		class Bishop : public Piece, public RayPiece, public NonKingPiece, public QBN_Piece {
		public:
			Bishop() : Piece{ Piece::BISHOP } {}
			Bishop(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_BISHOP : Piece::BLACK_BISHOP) } {}
			Bishop(Piece::piece_t val) : Piece{ val } {}
			Bishop(const Bishop &) = default;
			Bishop(Bishop &&) noexcept = default;
			virtual ~Bishop() noexcept = default;
			Bishop & operator=(const Bishop &) = default;
			Bishop & operator=(Bishop &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};

		class WhiteBishop : public Bishop {
			WhiteBishop() : Bishop{ Piece::WHITE_BISHOP } {}
			WhiteBishop(const WhiteBishop &) = default;
			WhiteBishop(WhiteBishop &&) noexcept = default;
			virtual ~WhiteBishop() noexcept = default;
			WhiteBishop & operator=(const WhiteBishop &) = default;
			WhiteBishop & operator=(WhiteBishop &&) noexcept = default;
		};

		class BlackBishop : public Bishop {
			BlackBishop() : Bishop{ Piece::BLACK_BISHOP } {}
			BlackBishop(const BlackBishop &) = default;
			BlackBishop(BlackBishop &&) noexcept = default;
			virtual ~BlackBishop() noexcept = default;
			BlackBishop & operator=(const BlackBishop &) = default;
			BlackBishop & operator=(BlackBishop &&) noexcept = default;
		};

		class Knight : public Piece, public NonKingPiece, public QBN_Piece {
		public:
			Knight() : Piece{ Piece::KNIGHT } {}
			Knight(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_KNIGHT : Piece::BLACK_KNIGHT) } {}
			Knight(Piece::piece_t val) : Piece{ val } {}
			Knight(const Knight &) = default;
			Knight(Knight &&) noexcept = default;
			virtual ~Knight() noexcept = default;
			Knight & operator=(const Knight &) = default;
			Knight & operator=(Knight &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};

		class WhiteKnight : public Knight {
			WhiteKnight() : Knight{ Piece::WHITE_KNIGHT } {}
			WhiteKnight(const WhiteKnight &) = default;
			WhiteKnight(WhiteKnight &&) noexcept = default;
			virtual ~WhiteKnight() noexcept = default;
			WhiteKnight & operator=(const WhiteKnight &) = default;
			WhiteKnight & operator=(WhiteKnight &&) noexcept = default;
		};

		class BlackKnight : public Knight {
			BlackKnight() : Knight{ Piece::BLACK_KNIGHT } {}
			BlackKnight(const BlackKnight &) = default;
			BlackKnight(BlackKnight &&) noexcept = default;
			virtual ~BlackKnight() noexcept = default;
			BlackKnight & operator=(const BlackKnight &) = default;
			BlackKnight & operator=(BlackKnight &&) noexcept = default;
		};

		class Rook : public Piece, public RayPiece, public NonKingPiece {
		public:
			Rook() : Piece{ Piece::ROOK } {}
			Rook(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_ROOK : Piece::BLACK_ROOK) } {}
			Rook(Piece::piece_t val) : Piece{ val } {}
			Rook(const Rook &) = default;
			Rook(Rook &&) noexcept = default;
			virtual ~Rook() noexcept = default;
			Rook & operator=(const Rook &) = default;
			Rook & operator=(Rook &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square) { return pushMask(square); }
		};
		
		class WhiteRook : public Rook {
			WhiteRook() : Rook{ Piece::WHITE_ROOK } {}
			WhiteRook(const WhiteRook &) = default;
			WhiteRook(WhiteRook &&) noexcept = default;
			virtual ~WhiteRook() noexcept = default;
			WhiteRook & operator=(const WhiteRook &) = default;
			WhiteRook & operator=(WhiteRook &&) noexcept = default;
		};

		class BlackRook : public Rook {
			BlackRook() : Rook{ Piece::BLACK_ROOK } {}
			BlackRook(const BlackRook &) = default;
			BlackRook(BlackRook &&) noexcept = default;
			virtual ~BlackRook() noexcept = default;
			BlackRook & operator=(const BlackRook &) = default;
			BlackRook & operator=(BlackRook &&) noexcept = default;
		};

		class Pawn : public Piece, public NonKingPiece {
		public:
			Pawn(uint8_t pawnColor) : Piece{ pawnColor } {}
			Pawn(const Pawn &) = default;
			Pawn(Pawn &&) noexcept = default;
			virtual ~Pawn() noexcept = default;
			Pawn & operator=(const Pawn &) = default;
			Pawn & operator=(Pawn &&) noexcept = default;
		};

		class WhitePawn : public Pawn {
		public:
			WhitePawn() : Pawn{ Piece::WHITE_PAWN } {}
			WhitePawn(const WhitePawn &) = default;
			WhitePawn(WhitePawn &&) noexcept = default;
			virtual ~WhitePawn() noexcept = default;
			WhitePawn & operator=(const WhitePawn &) = default;
			WhitePawn & operator=(WhitePawn &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square);
		};

		class BlackPawn : public Pawn {
		public:
			BlackPawn() : Pawn{ Piece::BLACK_PAWN } {}
			BlackPawn(const BlackPawn &) = default;
			BlackPawn(BlackPawn &&) noexcept = default;
			virtual ~BlackPawn() noexcept = default;
			BlackPawn & operator=(const BlackPawn &) = default;
			BlackPawn & operator=(BlackPawn &&) noexcept = default;

			// Identifies allToFen squares piece can move to as a BitBoard
			static BitBoard pushMask(BoardSquare square);

			static BitBoard captureMask(BoardSquare square);
		};

		template<typename PIECE_T>
		bool isRay() { return std::is_base_of<PIECE_T, RayPiece>(); }

		template<typename PIECE_T>
		bool isRay(const PIECE_T & piece) { return std::is_base_of<PIECE_T, RayPiece>(); }

	} // namespace Pieces
} // namespace forge