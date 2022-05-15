#include "AttackChecker.h"

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

	bool AttackChecker::isAttacked(const Board & board, BoardSquare square)
	{
		// 1.) --- Look for straight attacks (from Rooks and Queens) ---
		if (isAttackedByRook(board, square)) return true;

		// 2.) --- Look for diagonal attacks (from Bishops and Queens) ---
		if (isAttackedByBishop(board, square)) return true;

		// 3.) --- Look for knight attacks (from Knights) ---
		if (isAttackedByKnight(board, square)) return true;

		// 4.) --- Look for Pawn attacks (from Pawns) ---
		if (isAttackedByPawn(board, square)) return true;

		// 5.) --- Look for King attacks ---
		if (isAttackedByKing(board, square)) return true;

		// 6.) --- Piece at square is not being attacked ---
		return false;
	}

	bool AttackChecker::isKingAttacked(const Board & board, bool isWhiteKing)
	{
		BoardSquare kingSquare = (isWhiteKing ? board.whiteKing() : board.blackKing());

		return isAttacked(board, kingSquare);
	}

	bool AttackChecker::isAttackedByRook(const Board & board, BoardSquare square)
	{
		bool isWhite = board.isWhite(square);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		// --- Search Up ---
		FORGE_FIND_STRAIGHT_ATTACK(square, upOne, isTopRank);

		// --- Search Down ---
		FORGE_FIND_STRAIGHT_ATTACK(square, downOne, isBotRank);

		// --- Search Left ---
		FORGE_FIND_STRAIGHT_ATTACK(square, leftOne, isLeftFile);

		// --- Search Right ---
		FORGE_FIND_STRAIGHT_ATTACK(square, rightOne, isRightFile);

		// No attacks from up down left or right
		return false;
	}

	bool AttackChecker::isAttackedByBishop(const Board & board, BoardSquare square)
	{
		BoardSquare c;
		bool isWhite = board.isWhite(square);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		// --- Search Up Right ---
		FORGE_FIND_DIAGONAL_ATTACK(square, upRightOne, isTopRank, isRightFile);

		// --- Search Up Left ---
		FORGE_FIND_DIAGONAL_ATTACK(square, upLeftOne, isTopRank, isLeftFile);

		// --- Search Down Left ---
		FORGE_FIND_DIAGONAL_ATTACK(square, downLeftOne, isBotRank, isLeftFile);

		// --- Search Down Right ---
		FORGE_FIND_DIAGONAL_ATTACK(square, downRightOne, isBotRank, isRightFile);

		return false;
	}

	bool AttackChecker::isAttackedByKnight(const Board & board, BoardSquare square)
	{
		bool isWhite = board.isWhite(square);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		BoardSquare c;

		if (square.isKnight0InBounds()) {
			c = square.knight0();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight1InBounds()) {
			c = square.knight1();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight2InBounds()) {
			c = square.knight2();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight3InBounds()) {
			c = square.knight3();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight4InBounds()) {
			c = square.knight4();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight5InBounds()) {
			c = square.knight5();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight6InBounds()) {
			c = square.knight6();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (square.isKnight7InBounds()) {
			c = square.knight7();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}

		return false;
	}

	bool AttackChecker::isAttackedByPawn(const Board & board, BoardSquare square)
	{
		bool isWhite = board.isWhite(square);

		BoardSquare c;

		if (isWhite) {
			// Piece is WHITE and attacking pawns are BLACK.
			// Pawns will move DOWN.
			BitBoard theirs = board.blacks();

			if (square.isTopRank() == false && square.isRightFile() == false) {
				c = square.upRightOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
			if (square.isTopRank() == false && square.isLeftFile() == false) {
				c = square.upLeftOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
		}
		else {
			// Piece is BLACK and attacking pawns are WHITE.
			// Pawns will move UP.
			BitBoard theirs = board.whites();

			if (square.isBotRank() == false && square.isRightFile() == false) {
				c = square.downRightOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
			if (square.isBotRank() == false && square.isLeftFile() == false) {
				c = square.downLeftOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
		}

		return false;
	}

	bool AttackChecker::isAttackedByKing(const Board & board, BoardSquare square)
	{
		BoardSquare theirKing = (board.isWhite(square) ? board.blackKing() : board.whiteKing());

		// Calc distance between square and their king
		int8_t vertDist = abs((int8_t)square.row() - (int8_t)theirKing.row());
		int8_t horiDist = abs((int8_t)square.col() - (int8_t)theirKing.col());

		return vertDist <= 1 && horiDist <= 1;
	}
} // namespace forge