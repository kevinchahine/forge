#include "UciCurrLine.h"

#include <string>

using namespace std;

namespace forge
{
	namespace uci
	{
		std::ostream& operator<<(std::ostream& os, const UciCurrLine& line)
		{
			if (line.cpunr || line.moves) {
				os << "currline ";

				if (line.cpunr) {
					os << line.cpunr.get() << ' ';
				}
				if (line.moves) {
					for (const Move& m : line.moves.get()) {
						os << m << ' ';
					}
				}
			}

			return os;
		}
		std::istream& operator>>(std::istream& is, UciCurrLine& line)
		{
			is >> line.cpunr.get();

			string move;
			while (getline(is, move)) {
				line.moves->emplace_back(Move{ move });
			}

			return is;
		}
	} // namespace uci
} // namespace forge