#pragma once

#include <stdint.h>

namespace forge
{
	class MoveCounter
	{
	public:
		
		void reset() { count = 0; }

		bool isWhitesTurn() const { return count % 2 == 0; }
		bool isBlacksTurn() const { return !isWhitesTurn(); }

		const MoveCounter & operator++() { count++; return *this; }
		MoveCounter operator++(int) { auto temp = *this; count++; return temp; }

	public:
		int count = 0;
	};
} // namespace forge