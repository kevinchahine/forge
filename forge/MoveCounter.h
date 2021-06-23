#pragma once

#include <stdint.h>

namespace forge
{
	// Number of moves played
	// 0 - no pieces have moved yet. (Whites thinking)
	// 1 - white made its first move. (Blacks thinking)
	// 2 - black made its first move. (Whites thinking)
	// even numbers - (whites turn, whites thinking)
	// odd numbers - (blacks turn, blacks thinking)
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