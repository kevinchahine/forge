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

	bool AttackChecker::isKingAttacked(const Position & position, bool isWhiteKing)
	{
		// TODO: Currently ignores 3 fold repetition (draw by repetition)
		bool isAttacked = false;

		// --- White just moved (and now its Black's turn) ---
		if (isWhiteKing) {
			// See if White's King is attacked
			isAttacked = isKingAttacked(position.board(), position.board().m_whiteKing);
		}
		// --- Black just moved (and now its White's turn) ---
		else if (position.moveCounter().isWhitesTurn()) {
			// See if Black's King is attacked
			isAttacked = isKingAttacked(position.board(), position.board().m_blackKing);
		}

		return isAttacked;
	}

	bool AttackChecker::isKingAttacked(const Board & board, BoardSquare kingsSquare)
	{
		// 1.) --- Look for straight attacks (from Rooks and Queens) ---
		if (isKingAttackedStraight(board, kingsSquare)) return true;

		// 2.) --- Look for diagonal attacks (from Bishops and Queens) ---
		if (isKingAttackedDiagonal(board, kingsSquare)) return true;

		// 3.) --- Look for knight attacks (from Knights) ---
		if (isKingAttackedByKnight(board, kingsSquare)) return true;

		// 4.) --- Look for Pawn attacks (from Pawns) ---
		if (isKingAttackedByPawn(board, kingsSquare)) return true;

		// 5.) --- King is not attacked ---
		return false;
	}

	bool AttackChecker::isKingAttackedStraight(const Board & board, BoardSquare kingsSquare)
	{
		bool isWhite = board.isWhite(kingsSquare);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		// --- Search Up ---
		FORGE_FIND_STRAIGHT_ATTACK(kingsSquare, upOne, isTopRank);

		// --- Search Down ---
		FORGE_FIND_STRAIGHT_ATTACK(kingsSquare, downOne, isBotRank);

		// --- Search Left ---
		FORGE_FIND_STRAIGHT_ATTACK(kingsSquare, leftOne, isLeftFile);

		// --- Search Right ---
		FORGE_FIND_STRAIGHT_ATTACK(kingsSquare, rightOne, isRightFile);

		// No attacks from up down left or right
		return false;
	}

	bool AttackChecker::isKingAttackedDiagonal(const Board & board, BoardSquare kingsSquare)
	{
		BoardSquare c;
		bool isWhite = board.isWhite(kingsSquare);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		// --- Search Up Right ---
		FORGE_FIND_DIAGONAL_ATTACK(kingsSquare, upRightOne, isTopRank, isRightFile);

		// --- Search Up Left ---
		FORGE_FIND_DIAGONAL_ATTACK(kingsSquare, upLeftOne, isTopRank, isLeftFile);

		// --- Search Down Left ---
		FORGE_FIND_DIAGONAL_ATTACK(kingsSquare, downLeftOne, isBotRank, isLeftFile);

		// --- Search Down Right ---
		FORGE_FIND_DIAGONAL_ATTACK(kingsSquare, downRightOne, isBotRank, isRightFile);

		return false;
	}

	bool AttackChecker::isKingAttackedByKnight(const Board & board, BoardSquare kingsSquare)
	{
		bool isWhite = board.isWhite(kingsSquare);
		BitBoard theirs = (isWhite ? board.blacks() : board.whites());
		BitBoard ours = (isWhite ? board.whites() : board.blacks());

		BoardSquare c;

		if (kingsSquare.isKnight0InBounds()) {
			c = kingsSquare.knight0();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight1InBounds()) {
			c = kingsSquare.knight1();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight2InBounds()) {
			c = kingsSquare.knight2();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight3InBounds()) {
			c = kingsSquare.knight3();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight4InBounds()) {
			c = kingsSquare.knight4();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight5InBounds()) {
			c = kingsSquare.knight5();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight6InBounds()) {
			c = kingsSquare.knight6();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}
		if (kingsSquare.isKnight7InBounds()) {
			c = kingsSquare.knight7();
			if (board.occupied()[c] && theirs[c] && board.knights()[c]) {
				return true;
			}
		}

		return false;
	}

	bool AttackChecker::isKingAttackedByPawn(const Board & board, BoardSquare kingsSquare)
	{
		bool isWhite = board.isWhite(kingsSquare);

		BoardSquare c;

		if (isWhite) {
			BitBoard theirs = board.blacks();

			if (kingsSquare.isTopRank() == false && kingsSquare.isRightFile() == false) {
				c = kingsSquare.upRightOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
			if (kingsSquare.isTopRank() == false && kingsSquare.isLeftFile() == false) {
				c = kingsSquare.upLeftOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
		}
		else {
			BitBoard theirs = board.whites();

			if (kingsSquare.isBotRank() == false && kingsSquare.isRightFile() == false) {
				c = kingsSquare.downRightOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
			if (kingsSquare.isBotRank() == false && kingsSquare.isLeftFile() == false) {
				c = kingsSquare.downLeftOne();
				if (board.occupied()[c] && theirs[c] && board.pawns()[c]) {
					return true;
				}
			}
		}

		return false;
	}
} // namespace forge