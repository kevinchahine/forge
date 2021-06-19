#pragma once

#include "Position.h"
#include "Movers.h"
#include "MoveGenerator.h"

#include <vector>
#include <type_traits>

namespace forge
{
#ifndef FORGE_TRAVERSE
#define FORGE_TRAVERSE(START_SQUARE, BOUNDS_CHECK_FUNC, DIRECTION_FUNC, MOVE_PIECE_FUNC, CAPTURE_WITH_PIECE_FUNC) \
{ \
	BoardSquare s = START_SQUARE;	\
	\
	while (s.BOUNDS_CHECK_FUNC() == false) { \
		s = s.DIRECTION_FUNC(); \
		if (isEmpty[s]) { \
			moves.pushIfLegalQBNMove(position, Move{ START_SQUARE, s}); \
		} \
		else if (theirs[s]) { \
			moves.pushIfLegalQBNMove(position, Move{ START_SQUARE, s}); \
			break; \
		} \
		else { break; } \
	} \
} 
#endif // FORGE_TRAVERSE

#ifndef FORGE_TRAVERSE2
#define FORGE_TRAVERSE2(START_SQUARE, BOUNDS_CHECK_FUNC1, BOUNDS_CHECK_FUNC2, DIRECTION_FUNC, MOVE_PIECE_FUNC, CAPTURE_WITH_PIECE_FUNC) \
{ \
	BoardSquare s = START_SQUARE;	\
	\
	while (s.BOUNDS_CHECK_FUNC1() == false && s.BOUNDS_CHECK_FUNC2() == false) { \
		s = s.DIRECTION_FUNC(); \
		if (isEmpty[s]) { \
			moves.pushIfLegalQBNMove(position, Move{ START_SQUARE, s }); \
		} \
		else if (theirs[s]) { \
			moves.pushIfLegalQBNMove(position, Move{ START_SQUARE, s}); \
			break; \
		} \
		else { break; } \
	} \
} 
#endif // FORGE_TRAVERSE2

	inline void MoveGenerator::generatePawnMovesWhite(
		const Position & position,
		BoardSquare square,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard blacks = board.blacks();
		BoardSquare & s = square;			// Just a shorter alias

		// --- Moves (No captures) ---
		// Can pawn move up? 
		// Don't need bounds check here, because pawns will never legally be on top rank
		if (isEmpty[s.upOne()]) {	
			// Yes. Up one cell is empty.

			// --- Promotion ---
			// Can pawn promote?
			if (s.upOne().isTopRank()) {
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upOne(), pieces::whiteQueen });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upOne(), pieces::whiteRook });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upOne(), pieces::whiteBishop });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upOne(), pieces::whiteKnight }); 
			}
			else {
				// No promotion yet.
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upOne() });

				// Can it move a second time
				if (s.row() == 6 && isEmpty[s.up(2)]) {
					moves.pushIfLegalWhitePawnMove(position, Move{ s, s.up(2) });
				}
			}
		}

		// --- Captures (and Promotions) ---

		// Can pawn capture up left?
		// Don't need bounds check for rank because pawns will never be on top rank.
		if (s.isLeftFile() == false && blacks[s.upLeftOne()]) {
			// Yes Capture.

			// Would this capture result in a promotion?
			if (s.upLeftOne().isTopRank()) {
				// Yes promotion and capture
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upLeftOne(), pieces::whiteQueen });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upLeftOne(), pieces::whiteRook });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upLeftOne(), pieces::whiteBishop });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upLeftOne(), pieces::whiteKnight });
			}
			else {
				// No promotion. Only capture
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upLeftOne() });
			}
		}

		// Can pawn capture up right?
		// Don't need bounds check for rank because pawns will never be on top rank.
		if (s.isRightFile() == false && blacks[s.upRightOne()]) {
			// Would this capture result in a promotion?
			if (s.upRightOne().isTopRank()) {
				// Yes promotion and capture
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upRightOne(), pieces::whiteQueen });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upRightOne(), pieces::whiteRook });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upRightOne(), pieces::whiteBishop });
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upRightOne(), pieces::whiteKnight });
			}
			else {
				// No promotion without capture
				moves.pushIfLegalWhitePawnMove(position, Move{ s, s.upRightOne() });
			}
		}

		// TODO: Still need enpassent
	}

	inline void MoveGenerator::generatePawnMovesBlack(
		const Position & position,
		BoardSquare square,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these declarations should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard whites = board.whites();
		BoardSquare & s = square;			// Just a shorter alias

		// --- Moves and Promotios (No captures) ---
		// Can pawn move down?
		// Don't need bounds check here, because pawns will never legally be on bottom rank
		if (isEmpty[s.downOne()]) {
			// Yes. Down one cell is empty.

			// --- Promotion ---
			// Can pawn promote?
			if (s.downOne().isBotRank()) {
				// Yes promote pawn
				moves.pushIfLegalBlackPawnMove(position, Move{ s, s.downOne(), pieces::blackQueen });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, s.downOne(), pieces::blackRook });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, s.downOne(), pieces::blackBishop });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, s.downOne(), pieces::blackKnight });
			}
			else {
				// No promotion yet.
				moves.pushIfLegalBlackPawnMove(position, Move{ s, s.downOne() });

				// Can it move a second time
				if (s.row() == 1 && isEmpty[s.down(2)]) {
					moves.pushIfLegalBlackPawnMove(position, Move{ s, s.down(2) });
				}
			}
		}

		// --- Captures and Promotions ---

		// Can pawn capture down right?
		// Don't need bounds check for rank because pawns will never be on bottom rank.
		BoardSquare to = s.downLeftOne();
		if (s.isLeftFile() == false && whites[to]) {
			// Would this capture result in a promotion?
			if (to.isBotRank()) {
				// Yes promotion and capture
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackQueen });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackRook });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackBishop });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackKnight });
			}
			else {
				// No promotion. Only a capture
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to });
			}
		}

		to = s.downRightOne();
		if (s.isRightFile() == false && whites[to]) {
			// Would this capture result in a promotion?
			if (to.isBotRank()) {
				// Yes promotion and capture
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackQueen });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackRook });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackBishop });
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to, pieces::blackKnight });
			}
			else {
				// No promotion. Only a capture
				moves.pushIfLegalBlackPawnMove(position, Move{ s, to });
			}
		}

		// TODO: Still need enpassent
	}

	inline void MoveGenerator::generatePawnMoves(
		const Position & position, 
		BoardSquare square, 
		bool isWhite, 
		MoveList & moves)
	{
		if (isWhite) {
			generatePawnMovesWhite(position, square, moves);
		}
		else {
			generatePawnMovesBlack(position, square, moves);
		}
	}

	inline void MoveGenerator::generateRookMoves(
		const Position & position,
		BoardSquare rooksSquare,
		bool isWhite,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard theirs = (isWhite ? board.blacks() : board.whites()); // Opponents Pieces

		// --- Ups ---
		FORGE_TRAVERSE(rooksSquare, isTopRank, upOne, moveRook, captureWithRook);

		// --- Downs ---
		FORGE_TRAVERSE(rooksSquare, isBotRank, downOne, moveRook, captureWithRook);

		// --- Lefts ---
		FORGE_TRAVERSE(rooksSquare, isLeftFile, leftOne, moveRook, captureWithRook);

		// --- Rights ---
		FORGE_TRAVERSE(rooksSquare, isRightFile, rightOne, moveRook, captureWithRook);
	}

	inline void MoveGenerator::generateBishopMoves(
		const Position & position,
		BoardSquare square,
		bool isWhite,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard theirs = (isWhite ? board.blacks() : board.whites()); // Opponents Pieces

		// --- Up Rights ---
		FORGE_TRAVERSE2(square, isTopRank, isRightFile, upRightOne, moveQBNR, captureWithQBNR);

		// --- Up Lefts ---
		FORGE_TRAVERSE2(square, isTopRank, isLeftFile, upLeftOne, moveQBNR, captureWithQBNR);

		// --- Down Lefts ---
		FORGE_TRAVERSE2(square, isBotRank, isLeftFile, downLeftOne, moveQBNR, captureWithQBNR);

		// --- Down Rights ---
		FORGE_TRAVERSE2(square, isBotRank, isRightFile, downRightOne, moveQBNR, captureWithQBNR);
	}

	inline void MoveGenerator::generateKnightMoves(
		const Position & position,
		BoardSquare square,
		bool isWhite,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard theirs = (isWhite ? board.blacks() : board.whites()); // Opponents Pieces

		BoardSquare s = square;

		// TODO: OPTIMIZE: These if statements can be optimized
		if (square.isKnight0InBounds()) {
			Move move{ square, square.knight0() };

			if (isEmpty[s.knight0()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight0()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight1InBounds()) {
			Move move{ square, square.knight1() };

			if (isEmpty[s.knight1()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight1()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight2InBounds()) {
			Move move{ square, square.knight2() };

			if (isEmpty[s.knight2()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight2()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight3InBounds()) {
			Move move{ square, square.knight3() };

			if (isEmpty[s.knight3()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight3()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight4InBounds()) {
			Move move{ square, square.knight4() };

			if (isEmpty[s.knight4()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight4()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight5InBounds()) {
			Move move{ square, square.knight5() };

			if (isEmpty[s.knight5()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight5()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight6InBounds()) {
			Move move{ square, square.knight6() };

			if (isEmpty[s.knight6()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight6()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
		if (square.isKnight7InBounds()) {
			Move move{ square, square.knight7() };

			if (isEmpty[s.knight7()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight7()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
		}
	}

	inline void MoveGenerator::generateQueenMoves(
		const Position & position,
		BoardSquare square,
		bool isWhite,
		MoveList & moves)
	{
		// --- Alias some objects ---
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard theirs = (isWhite ? board.blacks() : board.whites()); // Opponents Pieces

		// --- Ups ---
		FORGE_TRAVERSE(square, isTopRank, upOne, moveQBNR, captureWithQBNR);

		// --- Downs ---
		FORGE_TRAVERSE(square, isBotRank, downOne, moveQBNR, captureWithQBNR);

		// --- Lefts ---
		FORGE_TRAVERSE(square, isLeftFile, leftOne, moveQBNR, captureWithQBNR);

		// --- Rights ---
		FORGE_TRAVERSE(square, isRightFile, rightOne, moveQBNR, captureWithQBNR);

		generateBishopMoves(position, square, isWhite, moves);
	}

	inline void MoveGenerator::generateKingMoves(
		const Position & position,
		BoardSquare square,
		bool isWhite,
		MoveList & moves)
	{
		const Board & board = position.board();
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());

#ifndef FORGE_KING
#define FORGE_KING(ORIGIN, DEST) \
		moves.pushIfLegalKingMove(position, Move{ ORIGIN, DEST });
#endif

		// --- UP MOVES ---
		if (square.isTopRank() == false) {
			// *** We can move up one ***
			FORGE_KING(square, square.upOne());

			if (square.isLeftFile() == false) {
				// *** We can move left ***
				FORGE_KING(square, square.upLeftOne());
			}
			if (square.isRightFile() == false) {
				// *** we can move right ***
				FORGE_KING(square, square.upRightOne());
			}
		}

		// --- MIDDLE MOVES ---
		if (square.isLeftFile() == false) {
			// *** We can move left ***
			FORGE_KING(square, square.leftOne());
		}
		if (square.isRightFile() == false) {
			// *** We can move right ***
			FORGE_KING(square, square.rightOne());
		}

		// --- DOWN MOVES ---
		if (square.isBotRank() == false) {
			// *** We can move down ***
			FORGE_KING(square, square.downOne());

			if (square.isLeftFile() == false) {
				// *** We can move left ***
				FORGE_KING(square, square.downLeftOne());
			}
			if (square.isRightFile() == false) {
				// *** we can move right ***
				FORGE_KING(square, square.downRightOne());
			}
		}
}

	template<typename PIECE_T>
	BitBoard MoveGenerator::getAttackers(const Position & position, BoardSquare square)
	{
		static_assert(
			is_base_of<pieces::Piece, PIECE_T>(),
			"Data type of PIECE_T must be one that inherits from forge::Piece");

		BitBoard bb = PIECE_T::moves(square);

		BitBoard pieces = position.board().pieces<PIECE_T>();

		return bb && pieces;
	}
} // namespace forge