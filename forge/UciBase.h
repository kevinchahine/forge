#pragma once

#include <iostream>

namespace forge
{
	namespace uci
	{
		class UciBase
		{
		public:
			UciBase(std::istream& in = std::cin, std::ostream& out = std::cout) :
				in(in),
				out(out) {}

		protected:
			std::ostream& out;
			std::istream& in;
		};
	} // namespace uci
} // namespace forge