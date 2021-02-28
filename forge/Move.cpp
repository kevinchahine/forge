#include "Move.h"

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
} // namespace forge