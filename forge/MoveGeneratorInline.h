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
			if (square.row() == 1) {
				// Promotion time!!! TODO: 
			}
			else {
				// No promotion yet.
				moves.emplace_back(Move{ square, square.upOne() }, position);
				moves.back().second.board().removePiece(square);
				moves.back().second.board().placeWhitePawn(square.upOne());

				// Can it move a second time
				if (square.row() == 6 && isEmpty[square.up(2)]) {
					moves.emplace_back(Move{ square, square.up(2) }, position);
					moves.back().second.board().removePiece(square);
					moves.back().second.board().placeWhitePawn(square.up(2));
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
		// --- (As long as method is inlined, these should be optimized away) ---
		const Board & board = position.board();
		BitBoard isEmpty = board.empty();
		BitBoard whites = board.whites();

		// Can pawn move down?
		if (isEmpty[square.downOne()]) {
			// Yes. Down one cell is empty.

			// --- Promotion ---
			// Can pawn move to top rank?
			if (square.row() == 6) {
				// Promotion time!!! TODO: 
			}
			else {
				// No promotion yet.
				moves.emplace_back(Move{ square, square.downOne() }, position);
				moves.back().second.board().removePiece(square);
				moves.back().second.board().placeBlackPawn(square.downOne());

				// Can it move a second time
				if (square.row() == 6 && isEmpty[square.down(2)]) {
					moves.emplace_back(Move{ square, square.down(2) }, position);
					moves.back().second.board().removePiece(square);
					moves.back().second.board().placeBlackPawn(square.down(2));
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
		
		// --- Ups ---
		// traverse upward. Make sure we don't go past the top boarder
		while (true) {
			s = s.upOne();

			// --- Moves ---
			// Can Rook move up to empty space?
			if (isEmpty[s]) {
				// Cell is empty so yes.
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().second.board().removePiece(rooksSquare);
				moves.back().second.board().placeWhiteRook(s);

			}
			// If rook moves up will it capture an opponents piece?
			else if (theirs[s]) {
				// It will capture an opponents pieces so yes.
				moves.emplace_back(Move{ rooksSquare, s }, position);
				moves.back().second.board().removePiece(rooksSquare);	// Pick up this Rook
				moves.back().second.board().removePiece(s);		// Remove captured Piece
				moves.back().second.board().placeWhiteRook(s);	// Place our Rook in its place
			}
			else { /* Up is a friendly piece */
				break; // A piece can't capture/jump it's own pieces.
			}

			// Have we hit the top row yet?
			if (s.isTopRank()) {
				// Yes, we've moved rook to top row, we shouldn't go any further.
				break;
			}
		}

		// --- Downs ---

		// --- Lefts ---

		// --- Rights ---
	}
} // namespace forge