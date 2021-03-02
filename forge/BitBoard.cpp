#include "BitBoard.h"

#include <Guten/Color.h>
#include <Guten/iocolor.h>

namespace forge
{
	std::bitset<64>::reference BitBoard::operator[](const BoardSquare & square)
	{
		return (*this)[square.val()];
	}

	bool BitBoard::operator[](const BoardSquare & square) const
	{
		return (*this)[square.val()];
	}

	std::ostream & operator<<(std::ostream & os, const BitBoard & bb)
	{
		for (size_t bit = 0; bit < 64; bit++) {
			if (bit % 8 == 0 && bit != 0)	os << '\'';

			os << static_cast<std::bitset<64>>(bb)[bit];
		}

		return os;
	}
} // namespace forge
