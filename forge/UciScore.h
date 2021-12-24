#pragma once

#include <iostream>

#include <boost/optional/optional.hpp>

namespace forge
{
	namespace uci
	{
		class UciScore
		{
		public:
			friend std::ostream & operator<<(std::ostream& os, const UciScore& score);
			friend std::istream & operator>>(std::istream& is, UciScore& score);

		public:
			// the score from the engine's point of view in centipawns.
			boost::optional<int> cp;
			// mate in y moves, not plies.
			// If the engine is getting mated use negative values for y.
			boost::optional<int> mate;
			// the score is just a lower bound.
			boost::optional<int> lowerbound;
			// the score is just an upper bound.
			boost::optional<int> upperbound;
		};
	} // namespace uci
} // namespace forge