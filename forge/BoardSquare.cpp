#include "BoardSquare.h"

std::ostream & forge::operator<<(std::ostream & os, const BoardSquare & pos)
{
	os << '(' << pos.row() << ", " << pos.col();

	return os;
}
