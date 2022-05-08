#pragma once

#include <iostream>
#include <string>

namespace forge
{
	// TODO: Optimize: Does compressing coordinates in BoardSquare actually lead to any kind of optimizations. 
	// Wouldn't it be better to simply store each coordinate in its own 8-bit int.
	class BoardSquare
	{
		friend class BitBoard;	// Gives BitBoard access to m_val

	public:
		BoardSquare() = default;
		BoardSquare(uint8_t val) :
			m_val(val) {}
		BoardSquare(int row, int col) :
			m_val(col | (row << 3)) {}
		BoardSquare(uint16_t row, uint16_t col) :
			m_val(col | (row << 3)) {}
		// file: ['a' - 'h'] (case insensitive)
		// rank: [ 1  -  8 ]
		BoardSquare(char file, char rank) :
			m_val((tolower(file) - 'a') | ((7 - (tolower(rank) - '1')) << 3)) {}
		BoardSquare(const std::string& coord);
		BoardSquare(const BoardSquare&) = default;
		BoardSquare(BoardSquare&&) noexcept = default;
		virtual ~BoardSquare() noexcept = default;
		BoardSquare& operator=(const BoardSquare&) = default;
		BoardSquare& operator=(BoardSquare&&) noexcept = default;
		//BoardSquare & operator++(int) { ++m_val; }
		//BoardSquare operator++() { }

		//bool operator==(const BoardSquare & bs) const { return this->m_val == bs.m_val; }
		//bool operator!=(const BoardSquare & bs) const { return this->m_val != bs.m_val; }

		bool operator==(BoardSquare bs) const { return this->m_val == bs.m_val; }
		bool operator!=(BoardSquare bs) const { return this->m_val != bs.m_val; }

		void fromString(const std::string& str);

		// Return coordinates in long algebraic notation
		std::string toString() const;

		// !!! Test This !!!
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
#ifdef _DEBUG
			if (rowCoord >= 8) {
				std::cout << "Error: " << __FUNCTION__ << " rowCoord = " << rowCoord
					<< " must be in range [0, 7]\n";
			}
#endif 
			m_val &= ~row_mask;			// clear row bits
			m_val |= (rowCoord << 3);	// set row bits to rowCoord
		}

		void col(uint8_t colCoord)
		{
#ifdef _DEBUG
			if (colCoord >= 8) {
				std::cout << "Error: " << __FUNCTION__ << " colCoord = " << colCoord
					<< " must be in range [0, 7]\n";
			}
#endif 
			m_val &= ~col_mask;			// clear col bits
			m_val |= (colCoord << 0);	// set col bits to colCoord
		}

		// Returns index of BoardSquare that can be used to access bits on a BitBoard or bitset
		// Range: [0, 63]
		// ex:
		//	BoardSquare bs;
		//	BitBoard bb;				
		//	
		//	bs.setCoords(4, 5);			// row 4, col 5
		//	uint8_t i = bs.index();		// Returns 4 * 8 + 5 = 37
		//	bb[i] = 1;					// Access bit using the index
		uint8_t index() const
		{
			return row() * 8 + col();
		}

		bool isValid() const
		{
			return !isInValid();
		}

		bool isInValid() const
		{
			return m_val & is_invalid_mask;
		}

		void setAsValid()
		{
			m_val &= ~is_invalid_mask; // set bit to 0
		}

		void setAsInvalid()
		{
			m_val |= is_invalid_mask;	// set bit to 1
		}

		static BoardSquare invalid() { BoardSquare bs; bs.setAsInvalid(); return bs; }

		// Returns true iff square refers to a light square. ex: a1, a3, a5
		bool isLightSquare() const { return (row() & 0b0001) == (col() & 0b0001); }	// '& 0b1' is same as 'modulus 2'
		// Returns true iff square referse to a dark square. ex: a2, a4, a6
		bool isDarkSquare() const { return !isLightSquare(); }

		// Is row coord, the top rank where black's pieces start
		// and white's pawns promote?
		bool isTopRank() const { return row() == 0; }
		// Is row coord, the bottom rank where white's pieces start
		// and black's pawns promote?
		bool isBotRank() const { return row() == 7; }
		// Is col coord the left most file
		bool isLeftFile() const { return col() == 0; }
		// Is col coord the right most file
		bool isRightFile() const { return col() == 7; }

		bool isKnight0InBounds() const { return row() >= 1 && col() <= 5; }
		bool isKnight1InBounds() const { return row() >= 2 && col() <= 6; }
		bool isKnight2InBounds() const { return row() >= 2 && col() >= 1; }
		bool isKnight3InBounds() const { return row() >= 1 && col() >= 2; }
		bool isKnight4InBounds() const { return row() <= 6 && col() >= 2; }
		bool isKnight5InBounds() const { return row() <= 5 && col() >= 1; }
		bool isKnight6InBounds() const { return row() <= 5 && col() <= 6; }
		bool isKnight7InBounds() const { return row() <= 6 && col() <= 5; }

		uint8_t val() const {
			return m_val;
		}

		// Warning: Only call if row != 0
		BoardSquare upOne() const { return m_val - 8; }
		BoardSquare up(uint8_t num) const { return m_val - num * 8; }

		// Warning: Only call if row != 7
		BoardSquare downOne() const { return m_val + 8; }
		BoardSquare down(uint8_t num = 1) const { return m_val + num * 8; }

		// Warning: Only call if col != 0
		BoardSquare leftOne() const { return BoardSquare(m_val - 1); }
		BoardSquare left(int8_t num = 1) const { return m_val - num * 1; }

		// Warning: Only call if col != 7
		BoardSquare rightOne() const { return BoardSquare(m_val + 1); }
		BoardSquare right(int8_t num = 1) const { return m_val + num * 1; }

		// Warning: Only call if row != 0 && col != 7
		BoardSquare upRightOne() const { return BoardSquare(m_val - 7); }
		BoardSquare upRight(int8_t num = 1) const { return m_val - num * 7; }

		// Warning: Only call if row != 0 && col != 0
		BoardSquare upLeftOne() const { return BoardSquare(m_val - 9); }
		BoardSquare upLeft(int8_t num = 1) const { return m_val - num * 9; }

		// Warning: Only call if row != 7 && col != 0
		BoardSquare downLeftOne() const { return BoardSquare(m_val + 7); }
		BoardSquare downLeft(int8_t num = 1) const { return m_val + num * 7; }

		// Warning: Only call if row != 7 && col != 7
		BoardSquare downRightOne() const { return BoardSquare(m_val + 9); }
		BoardSquare downRight(int8_t num = 1) const { return m_val + num * 9; }

		// Warning: Only call if inbounds
		BoardSquare knight0() const { return BoardSquare(m_val + 2 - 8); }
		BoardSquare knight1() const { return BoardSquare(m_val + 1 - 16); }
		BoardSquare knight2() const { return BoardSquare(m_val - 1 - 16); }
		BoardSquare knight3() const { return BoardSquare(m_val - 2 - 8); }
		BoardSquare knight4() const { return BoardSquare(m_val - 2 + 8); }
		BoardSquare knight5() const { return BoardSquare(m_val - 1 + 16); }
		BoardSquare knight6() const { return BoardSquare(m_val + 1 + 16); }
		BoardSquare knight7() const { return BoardSquare(m_val + 2 + 8); }

		//template<typename DIRECTION_T>
		//BoardSquare directionOf() const {
		//	return BoardSquare();
		//}

		// Rotates the BoardSquare 180 degrees. 
		// Same as rotating pieces without rotating the chess board.
		BoardSquare rotated() const { return BoardSquare(7 - row(), 7 - col()); }

		friend std::ostream& operator<<(std::ostream& os, const BoardSquare& pos)
		{
			os << pos.toString();

			return os;
		}

	private:
		// bits 0, 1, 2	- col coordinate
		// bits 3, 4, 5 - row coordinate
		// bit  6		- isInValid (0: valid, 1: invalid)
		// bit  7		- reserved
		// 0icccrrr
		uint8_t m_val = 0;

		static const uint8_t col_mask = 0b00'000'111;	// Has 1's for each col bit
		static const uint8_t row_mask = 0b00'111'000;	// Has 1's for each row bit
		static const uint8_t is_invalid_mask = 0b01'000'000;
	};
} // namespace forge

namespace std
{
	template<> struct hash<forge::BoardSquare>
	{
		size_t operator()(forge::BoardSquare const& bs) const noexcept
		{
			return bs.val();
		}
	};
} // namespace std