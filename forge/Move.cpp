#include "Move.h"

#include <string>
#include <sstream>

using namespace std;

namespace forge
{
	std::ostream & operator<<(std::ostream & os, const Move & move)
	{
		os << move.from() << move.to();

		Piece promotion = move.promotion();

		if (promotion != pieces::empty) {
			os << promotion;
		}

		return os;
	}

	string Move::toLAN() const
	{
		stringstream ss;

		ss << from() << to();

		register Piece promo = promotion();

		if (promo.isEmpty() == false) {
			ss << promo;
		}

		return ss.str();
	}
} // namespace forge