#pragma once

#include "Position.h"

namespace forge
{
	// Purpose: Determines if a piece is being attacked by opponent pieces
	class AttackChecker
	{
	public: 
		// Determines if square is being attacked by its opponents pieces
		// Determines color of piece automatically.
		// ex: If square points to a white piece, then isAttacked() will determine
		//	if any black pieces are attacking that square.
		// WARNING: Make sure 'square' cooresponds to an occupied square and
		//	not to an empty square.
		static bool isAttacked(const Board & board, BoardSquare square);

	private:
		// Determine if 'square' is being attacked by an opponents Rook or Queen
		// Determines the color of attacking player by the color of the piece at cell.
		// WARNING: Make sure 'square' does not coorespond to an empty square.
		static bool isAttackedByRook(const Board & board, BoardSquare square);
		// Determine if 'square' is being attacked by an opponents Bishop or Queen
		// * See comments of isStraightAttack()
		static bool isAttackedByBishop(const Board & board, BoardSquare square);
		// * See comments of isStraightAttack()
		// Determine if 'square' is being attacked by an opponents Knight
		static bool isAttackedByKnight(const Board & board, BoardSquare square);
		// Determine if 'square' is being attacked by an opponents Pawn
		// * See comments of isStraightAttack()
		static bool isKingAttackedByPawn(const Board & board, BoardSquare square);

	};
} // namespace forge