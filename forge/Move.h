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
			uint16_t v = (uint16_t) (m_val & from_mask).to_ulong();

			return BoardSquare((uint8_t) v);
		}
		
		void from(BoardSquare pos) 
		{
			std::bitset<16> posBits = pos.val();

			m_val = (m_val & ~from_mask) & posBits;
		}
		
		BoardSquare to() const 
		{
			uint16_t v = (uint16_t) (m_val & to_mask).to_ulong() >> 6;

			return BoardSquare((uint8_t) v);
		}
		
		void to(BoardSquare pos) 
		{
			std::bitset<16> posBits = pos.val() << 6;

			m_val = (m_val & ~to_mask) & posBits;
		}
		
		Piece promotion() const 
		{
			uint16_t v = (uint16_t) (m_val & promotion_mask).to_ulong() >> 12;

			return Piece((int8_t) v);
		}
		
		void promotion(BoardSquare piece) 
		{
			std::bitset<16> promotionBits = piece.val() << 12;

			m_val = (m_val & ~promotion_mask) & promotionBits;
		}

		// Determines if the Move object refers to only part of a move
		// where either the 'from' or 'to' component is specified
		// Useful with user interfaces where the user wants to specify their move in 
		// in 2 steps: One for the coordinate of the piece their moving (from) and
		// the second for the coordinate their moving to (to).
		// ex: User is prompted to make a move.
		// User selects 'e4'
		// Display highlights 'e4' square and all the legal moves for that piece
		// User selects 'e5'
		// Game applies the move 'e4e5'
		// When isPartial() returns true, to() and from() will be equal.
		bool isPartial() const { return to() == from(); }
		bool isPromotion() const { return promotion() != pieces::empty; }

		//std::string toPGN(const Board & board) const;

		// Returns Move as a string represented in 'Long Algebreic Notation'
		// ex: 'e4e5'		piece at e4 will move up to e5
		// ex: 'e7e8Q'		pawn at e7 will be promoted to Q on e8 (PAWN move only)
		// ex: 'e7d8Q'		pawn at e7 will be promoted to Q and capture piece on d8 (PAWN capture only)
		std::string toLAN() const;

		// prints move in long algebraic notation
		// To print using PGN notation use the .toPGN() method instead
		friend std::ostream & operator<<(std::ostream & os, const Move & move);

		// Converts stream characters in LAN format to a Move.
		// If only the first coordinate is specified, then move will be interpreted as a partial move
		// Parses token until a space character is reached or new line
		// Case insensitive
		// Do not put space between 'to' and 'from' coordinates
		// ex:	e4		# partial move
		//		e4e5	# to and from are both specified
		//		a7a8q	# pawn promotes to a Queen on a8
		//		e4 e5	# Bad, will only extract 'e4' from stream as a partial move.
		// WARNING: Does not support PGN because PGN requires a Board to remove ambiguities
		friend std::istream & operator>>(std::istream & is, Move & move);

	protected:
		// bits 0...5	- from [ row (3-bits) ][ col (3-bits) ]
		// bits 6...11	- to   [ row (3-bits) ][ col (3-bits) ]
		// bits 12...15 - promotion [ piece val (4-bits) ]
		std::bitset<16> m_val = 0;
	};
} // namespace forge