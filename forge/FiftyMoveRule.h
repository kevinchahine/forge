#pragma once

#include <iostream>

namespace forge
{
	// Keeps track of 50 move rule.
	class FiftyMoveRule
	{
	public:

		// Call at start of game.
		void reset() { halfMoveCount = -1; }

		// Call when a pawn has moved
		// Includes:
		//	- simple moves
		//	- captures
		//	- en passent
		//	- and promotions
		void pawnHasMoved() { reset(); }

		// Call when any piece has been captured
		void pieceCaptured() { reset(); }
		
		// Call this method whenever a move is made, even after calling
		// pawnHasMoved() or pieceCaptured()
		void update() { halfMoveCount++; }

		// Returns true iff no:
		//	- pawn moves or
		//	- captures
		// have been made in 50 full moves (100 half moves)
		bool isDraw() const { return halfMoveCount >= 100; }

	private:
		// Counts the number of half moves since the last time a:
		//	- pawn moved
		//	- a piece was captured
		int8_t halfMoveCount = 0;
	};
} // namespace forge