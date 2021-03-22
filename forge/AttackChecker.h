#pragma once

#include "Position.h"

namespace forge
{
	// Purpose: To determine if a piece is being attacked
	class AttackChecker
	{
	public: // ------------------------ IS KING ATTACKED ----------------------
		static bool isKingAttacked(const Position & position, bool isWhiteKing);

		// Determines if King is being attacked by its opponents pieces
		// Determines color of King automatically.
		// WARNING: Make sure 'kingsCell' does not coorespond to an empty square.
		static bool isKingAttacked(const Board & board, BoardSquare kingsCell);

		// TODO: This could be usefull maybe
		// Only applies
		//static bool isSquareAttacked(const Position & position, BoardSquare & square, bool isAttackedByWhite);

	private:
		// Determine if 'cell' is being attacked from some direction.
		// Determines the color of attacking player by the color of the piece at cell.
		// WARNING: Make sure 'cell' does not coorespond to an empty square.
		static bool isKingAttackedStraight(const Board & board, BoardSquare kingsSquare);
		// * See comments of isStraightAttack()
		static bool isKingAttackedDiagonal(const Board & board, BoardSquare kingsSquare);
		// * See comments of isStraightAttack()
		static bool isKingAttackedByKnight(const Board & board, BoardSquare kingsSquare);
		// * See comments of isStraightAttack()
		static bool isKingAttackedByPawn(const Board & board, BoardSquare kingsSquare);

	};
} // namespace forge