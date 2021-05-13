#pragma once

#include "BitBoard.h"

#include <iostream>
#include <bitset>

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
	class Piece
	{
	public:
		using piece_t = uint8_t;

		static const piece_t EMPTY = 0b0000;

		static const piece_t WHITE_KING = 0b0001;
		static const piece_t BLACK_KING = 0b1001;
		static const piece_t WHITE_QUEEN = 0b0010;
		static const piece_t BLACK_QUEEN = 0b1010;
		static const piece_t WHITE_BISHOP = 0b0011;
		static const piece_t BLACK_BISHOP = 0b1011;
		static const piece_t WHITE_KNIGHT = 0b0100;
		static const piece_t BLACK_KNIGHT = 0b1100;
		static const piece_t WHITE_ROOK = 0b0101;
		static const piece_t BLACK_ROOK = 0b1101;
		static const piece_t WHITE_PAWN = 0b0110;
		static const piece_t BLACK_PAWN = 0b1110;

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
			os << guten::color::push()
				<< guten::color::setfg(p.isBlack() ? guten::color::gray : guten::color::lightgreen)
				<< p.getCh()
				<< guten::color::pop();

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

		// Set piece based on character
		// Piece color is set to white by default
		void setCh(char ch, bool isWhite = true);

		bool isWhite() const { return m_val[3] == 0; }
		bool isBlack() const { return !isWhite(); }
		bool isEmpty() const { return m_val == EMPTY; }
		bool isOccupied() const { return !isEmpty(); }
		bool isKing() const { return (m_val == WHITE_KING) || (m_val == BLACK_KING); }
		bool isQueen() const { return (m_val == WHITE_QUEEN) || (m_val == BLACK_QUEEN); }
		bool isRook() const { return (m_val == WHITE_ROOK) || (m_val == BLACK_ROOK); }
		bool isBishop() const { return (m_val == WHITE_BISHOP) || (m_val == BLACK_BISHOP); }
		bool isKnight() const { return (m_val == WHITE_KNIGHT) || (m_val == BLACK_KNIGHT); }
		bool isPawn() const { return (m_val == WHITE_PAWN) || (m_val == BLACK_PAWN); }

		// Flips color of piece:
		// black -> white
		// white -> black
		void flipColor() { m_val.flip(3); }
		void makeWhite() { m_val[3] = 0; }
		void makeBlack() { m_val[3] = 1; }

	protected:
		std::bitset<8> m_val = 0;
	};

	namespace pieces {
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

			// Returns BitBoard containing all zeros
			static BitBoard moves(BoardSquare square);
		};

		class King : public Piece {
		public:
			King() : Piece{ Piece::KING } {}
			King(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_KING : Piece::BLACK_KING) } {}
			King(King &&) noexcept = default;
			virtual ~King() noexcept = default;
			King & operator=(const King &) = default;
			King & operator=(King &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			static BitBoard moves(BoardSquare square);
		};

		class Queen : public Piece {
		public:
			Queen() : Piece{ Piece::QUEEN } {}
			Queen(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_QUEEN : Piece::BLACK_QUEEN) } {}
			Queen(const Queen &) = default;
			Queen(Queen &&) noexcept = default;
			virtual ~Queen() noexcept = default;
			Queen & operator=(const Queen &) = default;
			Queen & operator=(Queen &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			static BitBoard moves(BoardSquare square);
		};

		class Bishop : public Piece {
		public:
			Bishop() : Piece{ Piece::BISHOP } {}
			Bishop(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_BISHOP : Piece::BLACK_BISHOP) } {}
			Bishop(const Bishop &) = default;
			Bishop(Bishop &&) noexcept = default;
			virtual ~Bishop() noexcept = default;
			Bishop & operator=(const Bishop &) = default;
			Bishop & operator=(Bishop &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			static BitBoard moves(BoardSquare square);
		};

		class Knight : public Piece {
		public:
			Knight() : Piece{ Piece::KNIGHT } {}
			Knight(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_KNIGHT : Piece::BLACK_KNIGHT) } {}
			Knight(const Knight &) = default;
			Knight(Knight &&) noexcept = default;
			virtual ~Knight() noexcept = default;
			Knight & operator=(const Knight &) = default;
			Knight & operator=(Knight &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			static BitBoard moves(BoardSquare square);
		};

		class Rook : public Piece {
		public:
			Rook() : Piece{ Piece::ROOK } {}
			Rook(bool isWhite) : Piece{ (isWhite ? Piece::WHITE_ROOK : Piece::BLACK_ROOK) } {}
			Rook(const Rook &) = default;
			Rook(Rook &&) noexcept = default;
			virtual ~Rook() noexcept = default;
			Rook & operator=(const Rook &) = default;
			Rook & operator=(Rook &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			static BitBoard moves(BoardSquare square);
		};

		class WhitePawn : public Piece {
		public:
			WhitePawn() : Piece{ Piece::WHITE_PAWN } {}
			WhitePawn(const WhitePawn &) = default;
			WhitePawn(WhitePawn &&) noexcept = default;
			virtual ~WhitePawn() noexcept = default;
			WhitePawn & operator=(const WhitePawn &) = default;
			WhitePawn & operator=(WhitePawn &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			BitBoard moves(BoardSquare square) const;
		}; 
		
		class BlackPawn : public Piece {
		public:
			BlackPawn() : Piece{ Piece::BLACK_PAWN } {}
			BlackPawn(const BlackPawn &) = default;
			BlackPawn(BlackPawn &&) noexcept = default;
			virtual ~BlackPawn() noexcept = default;
			BlackPawn & operator=(const BlackPawn &) = default;
			BlackPawn & operator=(BlackPawn &&) noexcept = default;

			// Identifies all squares piece can move to as a BitBoard
			BitBoard moves(BoardSquare square) const;
		};
	} // namespace Pieces
} // namespace forge