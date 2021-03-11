#pragma once

#include "BoardSquare.h"

#include <iostream>
#include <bitset>

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
		BitBoard(const BitBoard & bb) = default;
		BitBoard(const std::bitset<64> & bset) : std::bitset<64>(bset) {};
		~BitBoard() noexcept = default;

		BitBoard & operator=(const BitBoard &) = default;

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
		
		std::bitset<64>::reference operator[](const BoardSquare & square);

		bool operator[](const BoardSquare & square) const;

		friend std::ostream & operator<<(std::ostream & os, const BitBoard & bb);
	private:

	};
} // namespace forge