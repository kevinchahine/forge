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
	public: 

		static MoveList generateLegalMoves(const Position & position);

		static MoveList generateLegalMoves(const Position & position, bool isWhite);

		static MoveList generateLegalMovesFor(const Position & position, BoardSquare square);

	private:
		
		// do not call on empty cells
		// Make sure isWhite cooresponds to the color of the piece at cell
		static void generateLegalMovesFor(
			const Position & position,
			BoardSquare cell,
			bool isWhite,
			MoveList & dst);

	private:

		// Make sure these remain inlined to prevent overhead associated with passing function parameters

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
		static inline void generateKingMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);

	};
} // namespace forge

#include "MoveGeneratorInline.h"	// Includes inline definitions