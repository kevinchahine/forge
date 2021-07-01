#include "BitBoard.h"

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
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
