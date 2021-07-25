#include "BitBoard.h"

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
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

	void BitBoard::print(std::ostream & os) const
	{
		for (uint16_t row = 0; row < 8; row++) {
			for (uint16_t col = 0; col < 8; col++) {
				os << ((*this)[BoardSquare{ row, col }] ? '1' : '0');
			}
			os << '\n';
		}
		os << '\n';
	}
} // namespace forge
