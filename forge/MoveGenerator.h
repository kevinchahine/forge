#pragma once

#include "Position.h"
#include "MovePositionPair.h"
#include "MoveList.h"

namespace forge
{
	// A Pseudo Legal Move Generator.
	// Generates legal moves for a given Position but without accounting
	// for Positions that keep the king attacked.
	// Generated moves must be checked to see if the King is attacked.
	class MoveGenerator
	{
	public: // ------------------------ PSEUDO LEGAL MOVES --------------------

		// Figures out what player is making a move then generates
		// pseudo legal moves for that player.
		static MoveList generatePseudoMoves(const Position & position);

		// * See comments of other overloads
		static MoveList generatePseudoMoves(const Position & position, bool isWhite);

		// Generates all valid moves for the piece at the specified cell.
		// If that cell is empty, then resulting MoveList will be empty.
		// Does not account for who's turn it is.
		static MoveList generatePseudoMovesFor(const Position & position, BoardSquare cell);

	public: // ------------------------ IS KING ATTACKED ----------------------
		static bool isKingAttacked(const Position & position, bool isWhiteKing);

		// Determines if King is being attacked by its opponents pieces
		// Determines color of King automatically.
		// WARNING: Make sure 'cell' does not coorespond to an empty square.
		static bool isKingAttacked(const Board & board, BoardSquare kingsCell);

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

	private:
		// do not call on empty cells
		// Make sure isWhite cooresponds to the color of the piece at cell
		static void generatePseudoMovesFor(
			const Position & position, 
			BoardSquare cell, 
			bool isWhite,
			MoveList & dst);

	private:

		// Make sure these remain inlined to prevent overhead associated function with parameters

		// position - current game state. 
		// square - coordinate of piece that is moving.
		// moves - container where generated moves are to be placed
		// !!!WARNING - Make sure square referes to a piece of the proper type to avoid errors !!!
		static inline void generatePawnMovesWhite(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generatePawnMovesBlack(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generatePawnMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateRookMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateBishopMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateKnightMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateQueenMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
	};
} // namespace forge

#include "MoveGeneratorInline.h"	// Includes inline definitions