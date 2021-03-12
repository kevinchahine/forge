#pragma once

#include "Position.h"
#include "MovePositionPair.h"

#include <vector>

namespace forge
{
	using MoveList = std::vector<MovePositionPair>;

	// A Pseudo Legal Move Generator.
	// Generates legal moves for a given Position but without accounting
	// for Positions that keep the king attacked.
	// Generated moves must be checked to see if the King is attacked.
	class MoveGenerator
	{
	public:

		// Figures out what player is making a move then generates
		// pseudo legal moves for that player.
		static MoveList generatePseudoMoves(const Position & position);

		static MoveList generatePseudoMovesWhite(const Position & position);

		static MoveList generatePseudoMovesBlack(const Position & position);

		// Generates moves for the player whos turn it is.
		// Figures out which player is to move based on data in position
		static MoveList generatePseudoMovesCurr(const Position & position);

	private:

		// Make sure these remain inlined to prevent overhead associated function with parameters

		// position - current game state. 
		// square - coordinate of piece that is moving.
		// moves - container where generated moves are to be placed
		// !!!WARNING - Make sure square referes to a piece of the proper type to avoid errors !!!
		static inline void generatePawnMovesWhite(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generatePawnMovesBlack(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generateRookMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateBishopMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateKnightMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateQueenMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);

	};
} // namespace forge

#include "MoveGeneratorInline.h"	// Includes inline definitions