#include "BoardSquare.h"

using namespace std;

namespace forge
{
	string BoardSquare::toString() const
	{
		string s = "";

		s.push_back(col() + 'a');
		s.push_back(8 - row() + '0');

		return s;
	}
} // namespace forge

std::ostream & forge::operator<<(std::ostream & os, const BoardSquare & pos)
{
	os << pos.toString();

	return os;
}


