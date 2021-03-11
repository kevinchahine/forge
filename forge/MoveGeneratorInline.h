#pragma once

#include "Position.h"

#include <vector>
#include "MoveGenerator.h"

namespace forge
{
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
				// Promotion time!!! TODO: 
			}
			else {
				// No promotion yet.
				moves.emplace_back(Move{ square, square.upOne() }, position);

				moves.back().position.moveWhitePawn(square, square.upOne());

				// Can it move a second time
				if (square.row() == 6 && isEmpty[square.up(2)]) {
					moves.emplace_back(Move{ square, square.up(2) }, position);
					moves.back().position.moveWhitePawn(square, square.up(2));
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

		// Can pawn move down? // Don't need to 
		if (isEmpty[square.downOne()]) {
			// Yes. Down one cell is empty.

			// --- Promotion ---
			// Can pawn move to bottom rank?
			if (square.downOne().isBotRank()) {
				// Promotion time!!! TODO: 
			}
			else {
				// No promotion yet.
				moves.emplace_back(Move{ square, square.downOne() }, position);
				moves.back().position.moveBlackPawn(square, square.downOne());

				// Can it move a second time
				// - Must be still on row 1 and
				// - space must be empty
				if (square.row() == 1 && isEmpty[square.down(2)]) {
					moves.emplace_back(Move{ square, square.down(2) }, position);
					moves.back().position.moveBlackPawn(square, square.down(2));
				}
			}
		}

		// TODO: Still Need captures. Don't forget a capture can lead to a pomotion also.
		// TODO: Still need enpassent
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

		// Use s to traverse board.
		BoardSquare s = rooksSquare;

		// --- Any Direction ---
///#ifndef FORGE_ROOK_MOVES
///#define FORGE_ROOK_MOVES(BOUNDS_CHECK_FUNC, DIRECTION_FUNC)					\
///		while (s.BOUNDS_CHECK_FUNC() == false) {							\
///																			\
///			s = s.DIRECTION_FUNC();											\
///																			\
///			if (isEmpty[s]) {												\
///				moves.emplace_back(Move{ rooksSquare, s }, position);		\
///				moves.back().position.moveQBNR(rooksSquare, s);				\
///			}																\
///			else if (theirs[s]) {											\
///				moves.emplace_back(Move{ rooksSquare, s }, position);		\
///				moves.back().position.qbnrCapture(rooksSquare, s);			\
///			}																\
///			else { break; }													\
///		} 
///#endif 
		
		// --- Ups ---
		// traverse upward. Make sure we don't go past the top boarder
		s = rooksSquare; 
		while (s.isTopRank() == false) {
			s = s.upOne();
			if (isEmpty[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.moveQBNR(rooksSquare, s);
			}
			else if (theirs[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.qbnrCapture(rooksSquare, s);
			}
			else { break; }
		}

		// --- Downs ---
		// traverse downward. Make sure we don't go past the bottom boarder
		///FORGE_ROOK_MOVES(isBotRank, downOne);
		s = rooksSquare; 
		while (s.isBotRank() == false) {
				s = s.downOne();											
				if (isEmpty[s]) {
					moves.emplace_back(Move{ rooksSquare, s }, position);		
					moves.back().position.moveQBNR(rooksSquare, s);				
				}																
				else if (theirs[s]) {
					moves.emplace_back(Move{ rooksSquare, s }, position);		
					moves.back().position.qbnrCapture(rooksSquare, s);			
				}																
				else { break; }													
		}

		// --- Lefts ---
		///FORGE_ROOK_MOVES(isLeftFile, leftOne);
		s = rooksSquare;
		while (s.isLeftFile() == false) {
			s = s.leftOne();
			if (isEmpty[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.moveQBNR(rooksSquare, s);
			}
			else if (theirs[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.qbnrCapture(rooksSquare, s);
			}
			else { break; }
		}

		// --- Rights ---
		///FORGE_ROOK_MOVES(isRightFile, rightOne);
		s = rooksSquare;
		while (s.isRightFile() == false) {
			s = s.rightOne();
			if (isEmpty[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.moveQBNR(rooksSquare, s);
			}
			else if (theirs[s]) {
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().position.qbnrCapture(rooksSquare, s);
			}
			else { break; }
		}
	}
} // namespace forge