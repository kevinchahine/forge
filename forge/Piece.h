#pragma once

#include <iostream>
#include <bitset>

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
	class Piece
	{
	public:
		static const uint8_t EMPTY = 0b0000;

		static const uint8_t WHITE_KING		= 0b0001;
		static const uint8_t BLACK_KING		= 0b1001;
		static const uint8_t WHITE_QUEEN	= 0b0010;
		static const uint8_t BLACK_QUEEN	= 0b1010;
		static const uint8_t WHITE_BISHOP	= 0b0011;
		static const uint8_t BLACK_BISHOP	= 0b1011;
		static const uint8_t WHITE_KNIGHT	= 0b0100;
		static const uint8_t BLACK_KNIGHT	= 0b1100;
		static const uint8_t WHITE_ROOK		= 0b0101;
		static const uint8_t BLACK_ROOK		= 0b1101;
		static const uint8_t WHITE_PAWN		= 0b0110;
		static const uint8_t BLACK_PAWN		= 0b1110;


	public:
		Piece() : m_val(EMPTY) {}
		Piece(uint8_t value) : m_val(value) {}
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
	} // namespace Pieces
} // namespace forge