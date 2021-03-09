#pragma once

#include "BoardSquare.h"
#include "Piece.h"

#include <bitset>

namespace forge
{
	const std::bitset<16> from_mask =      0b0000'000000'111111;
	const std::bitset<16> to_mask =		   0b0000'111111'000000;
	const std::bitset<16> promotion_mask = 0b1111'000000'000000;

	class Move
	{
	public:
		Move() = default;
		Move(BoardSquare from, BoardSquare to) :
			m_val((to.val() << 6) | from.val()) {}
		Move(BoardSquare from, BoardSquare to, Piece promotion) :
			m_val((promotion.val() << 12) | (to.val() << 6) | (from.val())) {}
		// Constructs move based on string
		// string can be stored in PGN or LAN notation
		Move(std::string notation);
		Move(const Move &) = default;
		Move(Move &&) noexcept = default;
		~Move() noexcept = default;
		Move & operator=(const Move &) = default;
		Move & operator=(Move &&) noexcept = default;

		BoardSquare from() const 
		{
			register uint16_t v = (uint16_t) (m_val & from_mask).to_ulong();

			return BoardSquare((uint8_t) v);
		}
		
		void from(BoardSquare pos) 
		{
			register std::bitset<16> posBits = pos.val();

			m_val = (m_val & ~from_mask) & posBits;
		}
		
		BoardSquare to() const 
		{
			register uint16_t v = (uint16_t) (m_val & to_mask).to_ulong() >> 6;

			return BoardSquare((uint8_t) v);
		}
		
		void to(BoardSquare pos) 
		{
			register std::bitset<16> posBits = pos.val() << 6;

			m_val = (m_val & ~to_mask) & posBits;
		}
		
		Piece promotion() const 
		{
			register uint16_t v = (uint16_t) (m_val & promotion_mask).to_ulong() >> 12;

			return Piece((int8_t) v);
		}
		
		void promotion(BoardSquare piece) 
		{
			register std::bitset<16> promotionBits = piece.val() << 12;

			m_val = (m_val & ~promotion_mask) & promotionBits;
		}

		std::string toPGN() const;

		std::string toLAN() const;

		// prints move in long algebraic notation
		friend std::ostream & operator<<(std::ostream & os, const Move & move);

	protected:
		// bits 0...5	- from [ row (3-bits) ][ col (3-bits) ]
		// bits 6...11	- to   [ row (3-bits) ][ col (3-bits) ]
		// bits 12...15 - promotion [ signed ]
		std::bitset<16> m_val = 0;
	};
} // namespace forge