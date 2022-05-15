#include "forge/core/BoardSquare.h"

using namespace std;

namespace forge
{
	string BoardSquare::toString() const
	{
		string s = "";

		if (this->isValid()) {
			s.push_back(col() + 'a');
			s.push_back(8 - row() + '0');
		}
		else {
			s = "--";
		}

		return s;
	}
} // namespace forge