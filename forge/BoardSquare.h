#pragma once

#include "BitBoard.h"

#include <iostream>
#include <stdint.h>

namespace forge
{
	class BoardSquare
	{
		friend class BitBoard;	// Give BitBoard access to m_val

	public:
		BoardSquare() = default;
		BoardSquare(uint8_t val) :
			m_val(val) {}
		BoardSquare(uint8_t row, uint8_t col) :
			m_val(col | (row << 3)) {}
		BoardSquare(const std::string & coord);
		BoardSquare(const BoardSquare &) = default;
		BoardSquare(BoardSquare &&) noexcept = default;
		virtual ~BoardSquare() noexcept = default;
		BoardSquare & operator=(const BoardSquare &) = default;
		BoardSquare & operator=(BoardSquare &&) noexcept = default;

		bool operator==(const BoardSquare & bs) const { return this->m_val == bs.m_val; }
		bool operator!=(const BoardSquare & bs) const { return this->m_val != bs.m_val; }

		void fromString(const std::string & str);

		// Return coordinates in long algebraic notation
		std::string asString() const;

		// !!! TEst This !!!
		//BitBoard asBitBoard() const { 
		//	BitBoard b;		// All zeros
		//	b[m_val] = 1;	// Set only that bit to 1
		//	return b;		// Return
		//}

		void setCoord(uint8_t row, uint8_t col)
		{
			m_val = col | (row << 3);
		}

		uint8_t row() const
		{
			return m_val >> 3;		// shifts row bits while clearing col bits
		}

		uint8_t col() const
		{
			return m_val & col_mask;	// clears masks row bits leaving col only
		}

		void row(uint8_t rowCoord)
		{
			m_val &= ~row_mask;		// clear row bits
			m_val |= rowCoord;		// set row bits to rowCoord
		}

		void col(uint8_t colCoord)
		{
			m_val &= ~col_mask;		// clear col bits
			m_val |= (colCoord << 3);	// set col bits to colCoord
		}

		uint8_t val() const {
			return m_val;
		}

		// Warning: Only call if row != 0
		BoardSquare oneUp() const {	return BoardSquare(m_val - 8); }

		// Warning: Only call if row != 7
		BoardSquare oneDown() const { return BoardSquare(m_val + 8); }

		// Warning: Only call if col != 0
		BoardSquare oneLeft() const { return BoardSquare(m_val - 1); }

		// Warning: Only call if col != 7
		BoardSquare oneRight() const { return BoardSquare(m_val + 1); }

		// Warning: Only call if row != 0 && col != 7
		BoardSquare oneUpRight() const { return BoardSquare(m_val - 7); }

		// Warning: Only call if row != 0 && col != 0
		BoardSquare oneUpLeft() const { return BoardSquare(m_val - 9); }

		// Warning: Only call if row != 7 && col != 0
		BoardSquare oneDownLeft() const { return BoardSquare(m_val + 7); }

		// Warning: Only call if row != 7 && col != 7
		BoardSquare oneDownRight() const { return BoardSquare(m_val + 9); }

		// Warning: Only call if inbounds
		BoardSquare knight0() const { return BoardSquare(m_val + 2 - 8); }
		BoardSquare knight1() const { return BoardSquare(m_val + 1 - 16); }
		BoardSquare knight2() const { return BoardSquare(m_val - 1 - 16); }
		BoardSquare knight3() const { return BoardSquare(m_val - 2 - 8); }
		BoardSquare knight4() const { return BoardSquare(m_val - 2 + 8); }
		BoardSquare knight5() const { return BoardSquare(m_val - 1 + 16); }
		BoardSquare knight6() const { return BoardSquare(m_val + 1 + 16); }
		BoardSquare knight7() const { return BoardSquare(m_val + 2 + 8); }

		friend std::ostream & operator<<(std::ostream & os, const BoardSquare & pos);

	private:
		// bits 0, 1, 2	- col coordinate
		// bits 3, 4, 5 - row coordinate
		// bits 6, 7    - reserved
		// 00cccrrr
		uint8_t m_val = 0;

		static const uint8_t col_mask = 0b00'000'111;	// Has 1's for each col bit
		static const uint8_t row_mask = 0b00'111'000;	// Has 1's for each row bit
	};
} // namespace forge