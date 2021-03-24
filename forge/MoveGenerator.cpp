#include "MoveGenerator.h"

using namespace std;

namespace forge
{
#ifndef FORGE_FIND_STRAIGHT_ATTACK
#define FORGE_FIND_STRAIGHT_ATTACK(ORIGIN_CELL, MOVE_FUNC, BOUNDS_CHECK_FUNC) \
	{ \
		BoardSquare c = ORIGIN_CELL; \
		while (c.BOUNDS_CHECK_FUNC() == false) { \
			c = c.MOVE_FUNC(); \
			if (board.occupied()[c]) { \
				if (theirs[c]) { \
					if (board.m_rooks[c]) { \
						return true; \
					} \
				} \
				break; \
			} \
		} \
	}
#endif

#ifndef FORGE_FIND_DIAGONAL_ATTACK
#define FORGE_FIND_DIAGONAL_ATTACK(ORIGIN_CELL, MOVE_FUNC, BOUNDS_CHECK_FUNC1, BOUNDS_CHECK_FUNC2) \
	{ \
		BoardSquare c = ORIGIN_CELL; \
		while (c.BOUNDS_CHECK_FUNC1() == false && c.BOUNDS_CHECK_FUNC2() == false) { \
			c = c.MOVE_FUNC(); \
			if (board.occupied()[c]) { \
				if (theirs[c]) { \
					if (board.m_bishops[c]) { \
						return true; \
					} \
				} \
				break; \
			} \
		} \
	}
#endif
	
	MoveList MoveGenerator::generateLegalMoves(const Position & position)
	{
		return generateLegalMoves(position, position.moveCounter().isWhitesTurn());
	}

	MoveList MoveGenerator::generateLegalMoves(const Position & position, bool isWhite)
	{
		MoveList moves;
		moves.reserve(60);	// At any point in a chess game there will probably be about 35 - 38 legal moves

		// --- Alias some objects ---
		const Board & board = position.board();
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		// traverse each cell and look for white pieces
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare square(row, col);

				// Does this cell contain a white piece?
				if (ours[square] == 1) {
					generateLegalMovesFor(position, square, isWhite, moves);
				} // if (m_whites
			} // for (col
		} // for (row

		moves.shrink_to_fit();
		return moves;
	}

	MoveList MoveGenerator::generateLegalMovesFor(const Position & position, BoardSquare square)
	{
		MoveList moves;
		moves.reserve(7 + 7 + 7 + 6);	// At any point in a chess game there will probably be about 35 - 38 legal moves

		// 
		if (position.board().whites()[square] == 1) {
			generateLegalMovesFor(position, square, true, moves);
		}
		else if (position.board().blacks()[square] == 1) {
			generateLegalMovesFor(position, square, false, moves);
		}
		else {
			// Cell is empty, Don't generate anything	
		}

		moves.shrink_to_fit();
		return moves;
	}

	void MoveGenerator::generateLegalMovesFor(
		const Position & position, 
		BoardSquare square, 
		bool isWhite, 
		MoveList & moves)
	{
		// --- Alias some objects ---
		const Board & board = position.board();

		// Yes this is a black piece.
		if (board.isPawn(square)) {
			generatePawnMoves(position, square, isWhite, moves);
		}
		else if (board.isRook(square)) {
			generateRookMoves(position, square, isWhite, moves);
		}
		else if (board.isKnight(square)) {
			generateKnightMoves(position, square, isWhite, moves);
		}
		else if (board.isBishop(square)) {
			generateBishopMoves(position, square, isWhite, moves);
		}
		else if (board.isQueen(square)) {
			generateQueenMoves(position, square, isWhite, moves);
		}
		else if (board.isKing(square)) {
			// TODO: KING MOVES
		}
		else {
			// It must be an error.
			// May cause problems with UCI
			cout << "Uhhoo" << __FUNCTION__ << " line " << __LINE__ << '\n';
		}
	}

} // namespace forge
