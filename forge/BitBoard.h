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

		std::bitset<64>::reference operator[](const BoardSquare & square);
		bool operator[](const BoardSquare & square) const;

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

		// --- Ray Masks (As in not Lines) ---
		template<>
		static BitBoard mask<directions::Up>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Down>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Left>(BoardSquare center);
		template<>
		static BitBoard mask<directions::Right>(BoardSquare center);

		template<>
		static BitBoard mask<directions::UL>(BoardSquare center);
		template<>
		static BitBoard mask<directions::UR>(BoardSquare center);
		template<>
		static BitBoard mask<directions::DL>(BoardSquare center);
		template<>
		static BitBoard mask<directions::DR>(BoardSquare center);

		friend std::ostream & operator<<(std::ostream & os, const BitBoard & bb);
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
			std::assert("begin and end are not on the same lateral or diagonal");
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
		//static_assert(std::is_base_of<directions::LShape, LSHAPE_T>(), "LSHAPE_T must be type forge::directions::LShape");

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
		const BitBoard TOP = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;

		return TOP << (center.row() << 3);
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Vertical>(BoardSquare center)
	{
		const BitBoard LEFT = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		return LEFT << center.col();
	}

	template<>
	inline BitBoard BitBoard::mask<directions::MainDiagonal>(BoardSquare center)
	{
		// Diagonal direction: / 
		const BitBoard POS_DIAGONAL = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		BitBoard bb;

		int mainShift = 7 - (center.row() + center.col());

		if (mainShift > 0) {
			bb |= (POS_DIAGONAL >> (mainShift << 3));
		}
		else {
			bb |= (POS_DIAGONAL << (-mainShift << 3));
		}

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::OffDiagonal>(BoardSquare center)
	{
		const BitBoard NEG_DIAGONAL = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		BitBoard bb;

		int inverseShift = center.row() - center.col();

		if (inverseShift > 0) {
			bb |= (NEG_DIAGONAL << (inverseShift << 3));
		}
		else {
			bb |= (NEG_DIAGONAL >> (-inverseShift << 3));
		}

		return bb;
	}

	// --- Ray Masks ---

	template<>
	inline BitBoard BitBoard::mask<directions::Up>(BoardSquare center)
	{
		BitBoard bb = BitBoard::mask<directions::Vertical>(center);
		
		bb = bb >> center.col();

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Down>(BoardSquare center)
	{
		BitBoard bb = BitBoard::mask<directions::Vertical>(center);

		bb = bb << center.col();

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Left>(BoardSquare center)
	{
		BitBoard bb = BitBoard::mask<directions::Horizontal>(center);

		bb = bb << center.col();

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Right>(BoardSquare center)
	{
		BitBoard bb;
		
		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::UL>(BoardSquare center)
	{
		BitBoard bb;
		
		return bb;
	}
	
	template<>
	inline BitBoard BitBoard::mask<directions::UR>(BoardSquare center)
	{
		BitBoard bb;
		
		return bb;
	}
	
	template<>
	inline BitBoard BitBoard::mask<directions::DL>(BoardSquare center)
	{
		BitBoard bb;
		
		return bb;
	}
	
	template<>
	inline BitBoard BitBoard::mask<directions::DR>(BoardSquare center)
	{
		BitBoard bb;
		
		return bb;
	}
	
} // namespace forge