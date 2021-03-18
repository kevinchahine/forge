#pragma once

#include <iostream>

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
	class Piece
	{
	public:
		static const int8_t EMPTY = 0;

		static const int8_t WHITE_KING = 1;
		static const int8_t WHITE_QUEEN = 2;
		static const int8_t WHITE_BISHOP = 3;
		static const int8_t WHITE_KNIGHT = 4;
		static const int8_t WHITE_ROOK = 5;
		static const int8_t WHITE_PAWN = 6;

		static const int8_t BLACK_KING = -WHITE_KING;
		static const int8_t BLACK_QUEEN = -WHITE_QUEEN;
		static const int8_t BLACK_BISHOP = -WHITE_BISHOP;
		static const int8_t BLACK_KNIGHT = -WHITE_KNIGHT;
		static const int8_t BLACK_ROOK = -WHITE_ROOK;
		static const int8_t BLACK_PAWN = -WHITE_PAWN;

	public:
		Piece() : m_val(EMPTY) {}
		Piece(int8_t value) : m_val(value) {}
		Piece(const Piece &) = default;
		Piece(Piece &&) noexcept = default;
		virtual ~Piece() noexcept = default;
		Piece & operator=(const Piece &) = default;
		Piece & operator=(Piece &&) noexcept = default;

		bool operator==(const Piece & piece) const { return this->m_val == piece.m_val; }
		bool operator!=(const Piece & piece) const { return this->m_val != piece.m_val; }

		friend std::ostream & operator<<(std::ostream & os, const Piece & p)
		{
			int8_t v = p.val();

			os //<< guten::color::push()
				//<< guten::color::setfg(v < 0 ? guten::color::gray : guten::color::lightgreen)
				<< p.getCh();
				//<< guten::color::pop();

			return os;
		}

		int8_t val() const { return m_val; }
		int8_t & val() { return m_val; }

		char getCh() const
		{
			// Look up table
			// converts piece value to a printable character
			// index - piece value
			// output - character 
			static const int8_t pieceValToChar[] = {
				' ',	// 0  Empty
				'K',	// 1  White
				'Q',	// 2  White
				'B',	// 3  White
				'N',	// 4  White
				'R',	// 5  White
				'p',	// 6  White
			};

			return pieceValToChar[abs(this->m_val)];
		}

		bool isWhite() const { return m_val > 0; }
		bool isBlack() const { return m_val < 0; }
		bool isEmpty() const { return m_val == EMPTY; }
		bool isKing() const { return (m_val == WHITE_KING) || (m_val == BLACK_KING); }
		bool isQueen() const { return (m_val == WHITE_QUEEN) || (m_val == BLACK_QUEEN); }
		bool isRook() const { return (m_val == WHITE_ROOK) || (m_val == BLACK_ROOK); }
		bool isBishop() const { return (m_val == WHITE_BISHOP) || (m_val == BLACK_BISHOP); }
		bool isKnight() const { return (m_val == WHITE_KNIGHT) || (m_val == BLACK_KNIGHT); }
		bool isPawn() const { return (m_val == WHITE_PAWN) || (m_val == BLACK_PAWN); }

		// Flips color of piece:
		// black -> white
		// white -> black
		void flip() { m_val = -m_val; }

	protected:
		int8_t m_val = 0;
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