#pragma once

#include "Board.h"
#include "MoveCounter.h"
#include "FiftyMoveRule.h"

namespace forge
{
	class MoveGenerator;

	// Stores:
	//	- board (including pieces, castling rules, and enpassent)
	//	- 50 ply rule
	//	- move counter (tells whos turn it is)
	// Does not store:
	//	- time control
	//	- info about repetitions (Need game history for that)
	//	- game state: Win, Draw, Loose	(Need game history for that)
	class Position
	{
	public:
		// --- Declare Friend Classes ---
		friend MoveGenerator;

		void reset();

		// Removes all pieces
		void clear();

		// ----- Push Moves (non captures) -----
		template <typename PIECE_T> void pushPiece(Move move);
		template<> void pushPiece<pieces::King>(Move move);
		template<> void pushPiece<pieces::Queen>(Move move);
		template<> void pushPiece<pieces::Bishop>(Move move);
		template<> void pushPiece<pieces::Knight>(Move move);
		template<> void pushPiece<pieces::Rook>(Move move);
		template<> void pushPiece<pieces::WhitePawn>(Move move);
		template<> void pushPiece<pieces::BlackPawn>(Move move);
		template<> void pushPiece<pieces::Piece>(Move move);

		// ----- Captures (non push moves) -----
		///xxxtemplate <typename PIECE_T>	void capturePiece(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::King>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::Queen>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::Bishop>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::Knight>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::Rook>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::WhitePawn>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::BlackPawn>(Move capturingPiece);
		///xxxtemplate<> void capturePiece<pieces::Piece>(Move capturingPiece);

		// ----- PAWN MOVES -----

		// Intended to be used from class MoveGenerator to move pieces
		// efficiently. 
		// Automatically applies "50 move rule" and increments move counter.
		// !!! Warning: This method is intended to be more efficient than robust.
		// Calling incorrectly can cause errors. Follow these rules when calling.
		//	- 'from' should point to piece of player whos turn it is.
		//	- move should a simple move: (not a capture, not promotion, not castling, not enpassent)
		void moveWhitePawn(Move move);
		// WARNING: do not use for promotions. Use overload instead
		void moveWhitePawn(BoardSquare from, BoardSquare to);
		// *** See comments for moveWhitePawn(...)
		void moveBlackPawn(Move move);
		// WARNING: do not use for promotions. Use overload instead
		void moveBlackPawn(BoardSquare from, BoardSquare to);

		///xxxinline void captureWithWhitePawn(Move move);
		///xxxinline void captureWithBlackPawn(Move move);

		// ----- ROOK MOVES -----
		// ----- KNIGHT MOVES -----
		// ----- BISHOP MOVES -----
		// ----- QUEEN MOVES -----

		void moveRook(BoardSquare from, BoardSquare to);
		void moveRook(Move move);
		///xxxinline void captureWithRook(BoardSquare from, BoardSquare to);
		///xxxinline void captureWithRook(Move move);

		// Moves a Queen, Bishop, Knight or Rook of either color.
		// Make sure:
		//	- 'to' square is empty when calling this method
		//	- Not to be used with captures.
		//	- 'from' square is occupied by a Queen, Bishop, Knight or Rook (not empty)
		//	- 'from' should point to piece of player whos turn it is.
		void moveQBNR(BoardSquare from, BoardSquare to);
		void moveQBNR(Move move);

		// Captures a piece with a Queen, Bishop, Knight or Rook of either color.
		// Make sure:
		//	- 'to' square is occupied by an piece with color opposite capturing piece
		//	- 'from' square is occupied by a Queen, Bishop, Knight or Rook (not empty)
		//	- 'from' 
		//	- 'from' should point to piece of player whos turn it is.
		///xxxinline void captureWithQBNR(BoardSquare from, BoardSquare to);
		///xxxinline void captureWithQBNR(Move move);

		// ----- KING MOVES -----

		// TODO: Should account for castling
		void moveWhiteKing(BoardSquare to);

		// TODO: Should account for castling
		void moveBlackKing(BoardSquare to);

		///xxxinline void captureWithWhiteKing(BoardSquare to);
		///xxxinline void captureWithBlackKing(BoardSquare to);

		Board & board() { return m_board; }
		const Board & board() const { return m_board; }
		const FiftyMoveRule & fiftyMoveRule() const { return m_fiftyMoveRule; }
		const MoveCounter & moveCounter() const { return m_moveCounter; }

		// Only compares board.
		bool operator==(const Position & rhs) const
		{
			return
				(m_moveCounter.isWhitesTurn() == rhs.m_moveCounter.isWhitesTurn()) &&
				(m_board == rhs.m_board);
		}
		bool operator!=(const Position & rhs) const { return !(*this == rhs); }

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