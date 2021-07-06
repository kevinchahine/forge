#include "AttackerPair.h"

namespace forge
{
	void AttackerPair::print(const Board & board, std::ostream & os) const
	{
		os << this->size() << " attackers: ";

		for (const auto & attacker : *this) {
			os << board.at(attacker) << ' ' << attacker << ' ';
		}
	}
} // namespace forge