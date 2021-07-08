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

		std::bitset<64>::reference operator[](const BoardSquare & square) {
			return (*this)[square.val()];
		}
		bool operator[](const BoardSquare & square) const {
			return (*this)[square.val()];
		}

		template<typename DIRECTION_T>
		void shift(uint8_t numberOfCells) {
			static_assert(true,
				"This method is the unspeciallized template. "
				"Don't call this method. "
				"Instead call a fully specialized overload instead.");
		}

		// Shift all bits up a number of rows
		template<>
		void shift<directions::Up>(uint8_t numberOfRows) {
			(*this) = (*this) >> (numberOfRows << 3);
		}

		// Shift all bits down a number of rows
		template<>
		void shift<directions::Down>(uint8_t numberOfRows) {
			(*this) = (*this) << (numberOfRows << 3);
		}

		// Shift all bits left a number of cols.
		// Bits might overflow into downward rows 
		template<>
		void shift<directions::Left>(uint8_t numberOfCols) {
			(*this) = (*this) >> numberOfCols;
		}

		// Shift all bits left a number of cols.
		// Bits might overflow into upward rows 
		template<>
		void shift<directions::Right>(uint8_t numberOfCols) {
			(*this) = (*this) << numberOfCols;
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

		// Generates a BitBoard with 8 1s surrounding 'center'.
		// The 1s represent the squares a knight can move to from 'center'.
		template<>
		static BitBoard mask<directions::LShape>(BoardSquare center);

		// Generates a BitBoard with 1s in orientation of 'LINE_T'.
		// 'LINE_T' can be any type derived from forge::direction::Line.
		// ex: Horizontal, Vertical, MainDiagonal, OffDiagonal
		// The 1s represet the squares a Rook, Bishop or Queen can move to from 'center'.

		// --- Line Masks (As in not Rays) ---
		template<>
		static BitBoard mask<directions::Lateral>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Diagonal>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Horizontal>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Vertical>(BoardSquare center);
		template<>
		static BitBoard mask<directions::MainDiagonal>(BoardSquare center);
		template<>
		static BitBoard mask<directions::OffDiagonal>(BoardSquare center);

		// --- Lateral Ray Masks (As in not Lines) ---
		template<>
		static BitBoard mask<directions::Up>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Down>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Left>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Right>(BoardSquare center);

		// --- Diagonal Ray Masks (As in not Lines) ---
		template<>
		static BitBoard mask<directions::UL>(BoardSquare center);
		template<>
		static BitBoard mask<directions::UR>(BoardSquare center);
		template<>
		static BitBoard mask<directions::DL>(BoardSquare center);
		template<>
		static BitBoard mask<directions::DR>(BoardSquare center);

		friend std::ostream & operator<<(std::ostream & os, const BitBoard & bb);

		void print(std::ostream & os = std::cout) const;

	private:

	};

	template<typename RAY_DIRECTION_T>
	BitBoard BitBoard::mask(BoardSquare begin, BoardSquare end)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction like forge::directions::Up or forge::directions::UL");

#ifdef _DEBUG
		int rise = abs((int)begin.row() - (int)end.row());
		int run = abs((int)begin.col() - (int)end.col());

		if (rise == 0 || run == 0 || rise == run) {
		}
		else {
			std::cout << "Error: " << __FILE__ << " line " << __LINE__
				<< ": begin and end are not on the same lateral or diagonal\n";
		}
#endif

		BitBoard bb;

		// TODO: Optimize: This can really be optimized. 
		// We do not need a loop at all. Only bitwise operations.
		while (begin.operator!=(end)) {
			bb[begin] = 1;	// Set bit to 1

			begin = RAY_DIRECTION_T{}(begin);
		}

		return bb;
	}

	template<typename DIRECTION_T>
	BitBoard BitBoard::mask(BoardSquare center)
	{
		static_assert(true, "Call the explicit specializations instead of the primary call the method");

		BitBoard bb;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::LShape>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight0InBounds()) bb[center.knight0()] = 1;
		if (center.isKnight1InBounds()) bb[center.knight1()] = 1;
		if (center.isKnight2InBounds()) bb[center.knight2()] = 1;
		if (center.isKnight3InBounds()) bb[center.knight3()] = 1;
		if (center.isKnight4InBounds()) bb[center.knight4()] = 1;
		if (center.isKnight5InBounds()) bb[center.knight5()] = 1;
		if (center.isKnight6InBounds()) bb[center.knight6()] = 1;
		if (center.isKnight7InBounds()) bb[center.knight7()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Lateral>(BoardSquare center)
	{
		return
			BitBoard::mask<directions::Horizontal>(center) |
			BitBoard::mask<directions::Vertical>(center);
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Diagonal>(BoardSquare center)
	{
		return
			BitBoard::mask<directions::MainDiagonal>(center) |
			BitBoard::mask<directions::OffDiagonal>(center);
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Horizontal>(BoardSquare center)
	{
		BitBoard top = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;

		top.shift<directions::Down>(center.row());

		return top;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Vertical>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::MainDiagonal>(BoardSquare center)
	{
		// Diagonal direction: / 
		const BitBoard POS_DIAGONAL = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		BitBoard bb = POS_DIAGONAL;

		int mainShift = 7 - (center.row() + center.col());

		if (mainShift > 0) {
			bb.shift<directions::Up>(mainShift);
		}
		else {
			bb.shift<directions::Down>(-mainShift);
		}

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::OffDiagonal>(BoardSquare center)
	{
		const BitBoard NEG_DIAGONAL = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		BitBoard bb = NEG_DIAGONAL;

		int inverseShift = (int)center.row() - (int)center.col();

		if (inverseShift > 0) {
			bb.shift<directions::Down>(inverseShift);
		}
		else {
			bb.shift<directions::Up>(-inverseShift);
		}

		return bb;
	}

	// --- Ray Masks ---

	template<>
	inline BitBoard BitBoard::mask<directions::Up>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Up>(8 - center.row());
		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Down>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Down>(center.row());
		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Left>(BoardSquare center)
	{
		BitBoard top = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;

		top.shift<directions::Left>(8 - center.col());
		top.shift<directions::Down>(center.row());

		return top;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Right>(BoardSquare center)
	{
		BitBoard bottom = 0b11111111'00000000'00000000'00000000'00000000'00000000'00000000'00000000;

		bottom.shift<directions::Right>(center.col());
		bottom.shift<directions::Up>(7 - center.row());

		return bottom;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::UL>(BoardSquare center)
	{
		// Diagonal direction: \ 
		BitBoard negDiag = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		negDiag.shift<directions::Down>(7 - center.col());
		negDiag.shift<directions::Up>(7 - center.row());

		return negDiag;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::UR>(BoardSquare center)
	{
		// Diagonal direction: /  
		BitBoard posDiag = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		posDiag.shift<directions::Down>(center.col());
		posDiag.shift<directions::Up>(7 - center.row());

		return posDiag;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::DL>(BoardSquare center)
	{
		// Diagonal direction: / 
		BitBoard posDiag = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		posDiag.shift<directions::Up>(7 - center.col());
		posDiag.shift<directions::Down>(center.row());

		return posDiag;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::DR>(BoardSquare center)
	{
		// Diagonal direction: \ 
		BitBoard negDiag = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		negDiag.shift<directions::Up>(center.col());
		negDiag.shift<directions::Down>(center.row());

		return negDiag;
	}

} // namespace forge