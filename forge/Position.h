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

		// Removes all pieces
		void clear();

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

		// ----- PAWN MOVES -----

		// Intended to be used from class MoveGenerator to move pieces
		// efficiently. 
		// Automatically applies "50 move rule" and increments move counter.
		// !!! Warning: This method is intended to be more efficient than robust.
		// Calling incorrectly can cause errors. Follow these rules when calling.
		//	- 'from' should point to piece of player whos turn it is.
		//	- move should a simple move: (not a capture, not promotion, not castling, not enpassent)
		inline void moveWhitePawn(BoardSquare from, BoardSquare to);
		// *** See comments for moveWhitePawnSimple(...)
		inline void moveBlackPawn(BoardSquare from, BoardSquare to);

		inline void whitePawnCapture(BoardSquare from, BoardSquare to);

		inline void blackPawnCapture(BoardSquare from, BoardSquare to);

		inline void enPassentWhitePawn(BoardSquare from, BoardSquare to);

		inline void enPassentBlackPawn(BoardSquare from, BoardSquare to);

		// Also accounts for captures
		inline void promoteWhitePawn(BoardSquare from, BoardSquare to, Piece promotion);

		// Also accounts for captures
		inline void promoteBlackPawn(BoardSquare from, BoardSquare to, Piece promotion);

		// ----- ROOK MOVES -----
		// ----- KNIGHT MOVES -----
		// ----- BISHOP MOVES -----
		// ----- QUEEN MOVES -----

		// Moves a Queen, Bishop, Knight or Rook of either color.
		// Make sure:
		//	- 'to' square is empty when calling this method
		//	- Not to be used with captures.
		//	- 'from' square is occupied by a Queen, Bishop, Knight or Rook (not empty)
		//	- 'from' should point to piece of player whos turn it is.
		inline void moveQBNR(BoardSquare from, BoardSquare to);
		inline void moveQBNR(Move move);

		// Captures a piece with a Queen, Bishop, Knight or Rook of either color.
		// Make sure:
		//	- 'to' square is occupied by an piece with color opposite capturing piece
		//	- 'from' square is occupied by a Queen, Bishop, Knight or Rook (not empty)
		//	- 'from' 
		//	- 'from' should point to piece of player whos turn it is.
		inline void captureWithQBNR(BoardSquare from, BoardSquare to);
		inline void captureWithQBNR(Move move);

		// ----- KING MOVES -----

		inline void moveWhiteKing(BoardSquare to);

		inline void moveBlackKing(BoardSquare to);

		inline void whiteKingCapture(BoardSquare to);

		inline void blackKingCapture(BoardSquare to);

		inline void whiteKingCastle(BoardSquare to);

		inline void blackKingCastle(BoardSquare to);

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

#include "PositionInline.h"