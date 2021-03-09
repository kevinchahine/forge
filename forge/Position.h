#pragma once

#include "Board.h"
#include "MoveCounter.h"
#include "FiftyMoveRule.h"

namespace forge
{
	class MoveGenerator;

	// Stores:
	//	- board (including pieces, castling rules, and enpassent)
	//	- referee (50 ply rule, repetitions, calculates game state: Win, Draw, Loose)
	//	- Not time control
	class Position
	{
	public:
		// --- Declare Friend Classes ---
		friend MoveGenerator;

		void reset();

		Board & board() { return m_board; }
		const Board & board() const { return m_board; }

		// Makes a move.
		// returns whether move was valid or invalid
		void applyMoveFast(Move move);

		bool applyMoveSafe(Move move);

		// If the final position of a move is already known, then this
		// could be a more efficient alternative to applyMove(Move)
		// Works great for search algorithms because they can store the 
		// resulting positions of each move.
		// Simply copies position to *this
		// !!! Does not check for valid or invalid moves. 
		// Passing an invalid board may have unexpected results.
		void applyMove(const Position & position);

		const MoveCounter & moveCounter() const { return m_moveCounter; }

	protected:
		Board m_board;

		// If in 50 moves no captures have been made and no pawns moved,
		// then its a draw.
		FiftyMoveRule m_fiftyMoveRule;

		// Number of moves played
		// 0 - no pieces have moved yet. (Whites thinking)
		// 1 - white made its first move. (Blacks thinking)
		// even numbers - (whites turn, whites thinking)
		// odd numbers - (blacks turn, blacks thinking)
		MoveCounter m_moveCounter;
	};

} // namespace forge