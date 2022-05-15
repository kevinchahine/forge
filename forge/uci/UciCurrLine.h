#pragma once

#include "Move.h"

#include <boost/optional/optional.hpp>

#include <iostream>
#include <vector>

namespace forge
{
	namespace uci
	{
		class UciCurrLine
		{
		public:
			friend std::ostream& operator<<(std::ostream& os, const UciCurrLine& uciCurrLine);
			friend std::istream& operator>>(std::istream& is, UciCurrLine& uciCurrLine);

		public:
			boost::optional<int> cpunr;
			boost::optional<std::vector<Move>> moves;
		};
	} // namespace uci
} // namespace forge