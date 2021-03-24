#pragma once

#include "Position.h"

#include <vector>
#include "MoveGenerator.h"

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
			moves.emplace_back(Move{ START_SQUARE, s }, position); \
			moves.back().position.MOVE_PIECE_FUNC(START_SQUARE, s); \
		} \
		else if (theirs[s]) { \
			moves.emplace_back(Move{ START_SQUARE, s}, position); \
			moves.back().position.CAPTURE_WITH_PIECE_FUNC(START_SQUARE, s); \
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
			moves.pushIfLegalQBNCapture(position, Move{ START_SQUARE, s}); \
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

		// Can pawn move up? 
		if (isEmpty[square.upOne()]) {
			// Yes. Up one cell is empty.

			// --- Promotion ---
			// Can pawn move to top rank?
			if (square.upOne().isTopRank()) {
				// TODO: Promotion time!!! 
				cout << __FUNCTION__ << "Promotion time not implemented\n";
			}
			else {
				// No promotion yet.
				moves.pushIfLegalWhitePawnMove(position, Move{ square, square.upOne() });

				// Can it move a second time
				if (square.row() == 6 && isEmpty[square.up(2)]) {
					moves.pushIfLegalWhitePawnMove(position, Move{ square, square.up(2) });
				}
			}
		}

		// TODO: Still Need captures. Don't forget a capture can lead to a pomotion also.
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

		// Can pawn move down?
		if (isEmpty[square.downOne()]) {
			// Yes. Down one cell is empty.

			// --- Promotion ---
			// Can pawn move to bottom rank?
			if (square.downOne().isBotRank()) {
				// TODO: Promotion time!!! 
				cout << __FUNCTION__ << "Promotion time not implemented\n";
			}
			else {
				// No promotion yet.
				moves.pushIfLegalBlackPawnMove(position, Move{ square, square.downOne() });
				
				// Can it move a second time
				if (square.row() == 1 && isEmpty[square.down(2)]) {
					moves.pushIfLegalBlackPawnMove(position, Move{ square, square.down(2) });
				}
			}
		}

		// TODO: Still Need captures. Don't forget a capture can lead to a pomotion also.
		// TODO: Still need enpassent
	}

	inline void MoveGenerator::generatePawnMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves)
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
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight1InBounds()) {
			Move move{ square, square.knight1() };

			if (isEmpty[s.knight1()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight1()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight2InBounds()) {
			Move move{ square, square.knight2() };

			if (isEmpty[s.knight2()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight2()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight3InBounds()) {
			Move move{ square, square.knight3() };

			if (isEmpty[s.knight3()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight3()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight4InBounds()) {
			Move move{ square, square.knight4() };

			if (isEmpty[s.knight4()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight4()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight5InBounds()) {
			Move move{ square, square.knight5() };

			if (isEmpty[s.knight5()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight5()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight6InBounds()) {
			Move move{ square, square.knight6() };

			if (isEmpty[s.knight6()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight6()]) {
				moves.pushIfLegalQBNCapture(position, move);
			}
		}
		if (square.isKnight7InBounds()) {
			Move move{ square, square.knight7() };

			if (isEmpty[s.knight7()]) {
				moves.pushIfLegalQBNMove(position, move);
			}
			else if (theirs[s.knight7()]) {
				moves.pushIfLegalQBNCapture(position, move);
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

} // namespace forge