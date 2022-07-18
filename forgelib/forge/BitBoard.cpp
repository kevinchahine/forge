#include <forge/BitBoard.h>

using namespace std;

// TODO: SHOULD NOT NEED GUTEN
//#include <Guten/color/Color.h>
//#include <Guten/termcolor/termcolor.hpp>

namespace forge
{
	BitBoard BitBoard::rotated() const {
		// TODO: There should be a way to optimize this
		// Consider that this whole Bitboard can fit in a single register and these instructions
		// will run in CPU time. Any optimizations will probably give a 2% speed up and might not be
		// worth it. Just don't waste your time.
		BitBoard bb;

		const size_t size = (*this).size();
		const size_t size_1 = size - 1;

		for (size_t bit = 0; bit < size; bit++) {
			bb[bit] = (*this)[size_1 - bit];
		}

		return bb;
	}

	// -------------------------------- EXPLICIT SPECIALIZATIONS --------------

	// Shift allToFen bits up a number of rows
	template<> void BitBoard::shift<directions::Up>(uint8_t numberOfRows) {
		(*this) = (*this) >> (numberOfRows << 3);
	}

	// Shift allToFen bits down a number of rows
	template<> void BitBoard::shift<directions::Down>(uint8_t numberOfRows) {
		(*this) = (*this) << (numberOfRows << 3);
	}

	// Shift allToFen bits left a number of cols.
	// Bits might overflow into downward rows 
	template<> void BitBoard::shift<directions::Left>(uint8_t numberOfCols) {
		(*this) = (*this) >> numberOfCols;
	}

	// Shift allToFen bits left a number of cols.
	// Bits might overflow into upward rows 
	template<> void BitBoard::shift<directions::Right>(uint8_t numberOfCols) {
		(*this) = (*this) << numberOfCols;
	}

	// ---------------------- MASK --------------------------------------------
	template<>
	BitBoard BitBoard::mask<directions::Knight0>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight0InBounds()) bb[center.knight0()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight1>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight1InBounds()) bb[center.knight1()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight2>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight2InBounds()) bb[center.knight2()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight3>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight3InBounds()) bb[center.knight3()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight4>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight4InBounds()) bb[center.knight4()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight5>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight5InBounds()) bb[center.knight5()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight6>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight6InBounds()) bb[center.knight6()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Knight7>(BoardSquare center)
	{
		BitBoard bb;

		if (center.isKnight7InBounds()) bb[center.knight7()] = 1;

		return bb;
	}

	template<>
	BitBoard BitBoard::mask<directions::Horizontal>(BoardSquare center)
	{
		BitBoard top = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;

		top.shift<directions::Down>(center.row());

		return top;
	}

	template<>
	BitBoard BitBoard::mask<directions::Vertical>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	BitBoard BitBoard::mask<directions::Lateral>(BoardSquare center)
	{
		return
			BitBoard::mask<directions::Horizontal>(center) |
			BitBoard::mask<directions::Vertical>(center);
	}

	template<>
	BitBoard BitBoard::mask<directions::MainDiagonal>(BoardSquare center)
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
	BitBoard BitBoard::mask<directions::OffDiagonal>(BoardSquare center)
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

	template<>
	BitBoard BitBoard::mask<directions::Diagonal>(BoardSquare center)
	{
		return
			BitBoard::mask<directions::MainDiagonal>(center) |
			BitBoard::mask<directions::OffDiagonal>(center);
	}

	// --- Ray Masks ---

	template<>
	BitBoard BitBoard::mask<directions::Up>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Up>(8 - center.row());
		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	BitBoard BitBoard::mask<directions::Down>(BoardSquare center)
	{
		BitBoard left = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

		left.shift<directions::Down>(center.row());
		left.shift<directions::Right>(center.col());

		return left;
	}

	template<>
	BitBoard BitBoard::mask<directions::Left>(BoardSquare center)
	{
		BitBoard top = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;

		top.shift<directions::Left>(8 - center.col());
		top.shift<directions::Down>(center.row());

		return top;
	}

	template<>
	BitBoard BitBoard::mask<directions::Right>(BoardSquare center)
	{
		BitBoard bottom = 0b11111111'00000000'00000000'00000000'00000000'00000000'00000000'00000000;

		bottom.shift<directions::Right>(center.col());
		bottom.shift<directions::Up>(7 - center.row());

		return bottom;
	}

	template<>
	BitBoard BitBoard::mask<directions::UL>(BoardSquare center)
	{
		// Diagonal direction: \ ..
		BitBoard negDiag = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		negDiag.shift<directions::Down>(7 - center.col());
		negDiag.shift<directions::Up>(7 - center.row());

		return negDiag;
	}

	template<>
	BitBoard BitBoard::mask<directions::UR>(BoardSquare center)
	{
		// Diagonal direction: /  
		BitBoard posDiag = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		posDiag.shift<directions::Down>(center.col());
		posDiag.shift<directions::Up>(7 - center.row());

		return posDiag;
	}

	template<>
	BitBoard BitBoard::mask<directions::DL>(BoardSquare center)
	{
		// Diagonal direction: / 
		BitBoard posDiag = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

		posDiag.shift<directions::Up>(7 - center.col());
		posDiag.shift<directions::Down>(center.row());

		return posDiag;
	}

	template<>
	BitBoard BitBoard::mask<directions::DR>(BoardSquare center)
	{
		// Diagonal direction: \ ..
		BitBoard negDiag = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

		negDiag.shift<directions::Up>(center.col());
		negDiag.shift<directions::Down>(center.row());

		return negDiag;
	}

	template<>
	BitBoard BitBoard::mask<directions::LShape>(BoardSquare center)
	{
		BitBoard bb;

		// TODO: OPTIMIZE: This can be optimized to bitwise operations only
		// no ifs. Also consider making it inline

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

	std::ostream & operator<<(std::ostream & os, const BitBoard & bb)
	{
		for (size_t bit = 0; bit < 64; bit++) {
			if (bit % 8 == 0 && bit != 0)	os << '\'';

			os << static_cast<std::bitset<64>>(bb)[bit];
		}

		return os;
	}
	
	// TODO: Should not need this	
	//guten::core::Matrix BitBoard::toMat() const
	//{
	//	guten::core::Matrix mat{ 8, 8 };
	//
	//	for (uint16_t row = 0; row < 8; row++) {
	//		for (uint16_t col = 0; col < 8; col++) {
	//			BoardSquare bs{ row, col };
	//			mat.at(row, col) = ((*this)[bs] ? '1' : '0');
	//		}
	//	}
	//
	//	return mat;
	//}

	void BitBoard::print(std::ostream & os) const
	{
		for (uint16_t row = 0; row < 8; row++) {
			for (uint16_t col = 0; col < 8; col++) {
				BoardSquare bs{ row, col };
				os << ((*this)[bs] ? '1' : '0');
			}
			os << endl;
		}
		os << endl;
	}
} // namespace forge
