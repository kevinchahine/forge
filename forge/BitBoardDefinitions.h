#pragma once

#include "BitBoard.h"

namespace forge
{

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
		// We do not need a loop at allToFen. Only bitwise operations.
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
	inline BitBoard BitBoard::mask<directions::Knight0>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight0InBounds()) bb[center.knight0()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight1>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight1InBounds()) bb[center.knight1()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight2>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight2InBounds()) bb[center.knight2()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight3>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight3InBounds()) bb[center.knight3()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight4>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight4InBounds()) bb[center.knight4()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight5>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight5InBounds()) bb[center.knight5()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight6>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight6InBounds()) bb[center.knight6()] = 1;

		return bb;
	}

	template<>
	inline BitBoard BitBoard::mask<directions::Knight7>(BoardSquare center)
	{
		BitBoard bb;

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