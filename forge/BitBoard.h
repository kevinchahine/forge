#pragma once

#include "Direction.h"

#include <iostream>
#include <bitset>

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

namespace forge
{
	// --- Forward Declarations ---
	class BoardSquare;

	/*
	Each bit coordesponds to a cell on the chess board
		 0  1  2  3  4  5  6  7
		 8  9 10 11 12 13 14 15
		16 17 18 19 20 21 22 23
		24 25 26 27 28 29 30 31
		32 33 34 35 36 37 38 39
		40 41 42 43 44 45 46 47
		48 49 50 51 52 53 54 55
		56 57 58 59 60 61 62 63
	*/
	class BitBoard : public std::bitset<64>
	{
	public:
		BitBoard() = default;
		BitBoard(unsigned long long l) : std::bitset<64>(l) {};
		BitBoard(const BitBoard& bb) = default;
		BitBoard(const std::bitset<64>& bset) : std::bitset<64>(bset) {};
		~BitBoard() noexcept = default;
		BitBoard& operator=(const BitBoard&) = default;

		std::bitset<64>::reference operator[](size_t i) {
			return static_cast<bitset<64> &>(*this)[i];
		}
		bool operator[](size_t i) const {
			return this->std::bitset<64>::operator[](i);
		}

		std::bitset<64>::reference operator[](int i) {
			return static_cast<bitset<64> &>(*this)[i];
		}
		bool operator[](int i) const {
			return this->std::bitset<64>::operator[](i);
		}

		std::bitset<64>::reference operator[](const BoardSquare& square) {
			return (*this)[square.val()];
		}
		bool operator[](const BoardSquare& square) const {
			return (*this)[square.val()];
		}

		template<typename DIRECTION_T>
		void shift(uint8_t numberOfCells) {
			static_assert(true,
				"This method is the unspeciallized template. "
				"Don't call this method. "
				"Instead call a fully specialized overload instead.");
		}

		// Draws a line of 1s between begin and end.
		// Inclusive on begin
		// Exclusive on end
		// DIRECTION_T should be of a ray direction type
		//	ex: Up, Down, UL, DL. Not Horizontal or Vertical
		//	See Direction.h
		// Make sure the path between begin and end are in the same direction
		//	as RAY_DIRECTION_T.
		template<typename RAY_DIRECTION_T>
		static BitBoard mask(BoardSquare begin, BoardSquare end);

		template<typename DIRECTION_T>
		static BitBoard mask(BoardSquare center);

		friend std::ostream& operator<<(std::ostream& os, const BitBoard& bb);

		void print(std::ostream& os = std::cout) const;

	private:
	}; // class BitBoard
} // namespace forge

// --- Inject hash into std namespace ---
namespace std
{
	template<> struct hash<forge::BitBoard>
	{
	public:
		std::size_t operator()(const forge::BitBoard& b) const noexcept
		{
			return std::hash<std::bitset<64>>{}(b);
		}
	};
}

#include "BitBoardDefinitions.h"