#pragma once

#include "Position.h"

#include <vector>

namespace forge
{
	using MoveList = std::vector<std::pair<Move, Position>>;

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

	private:

		static void generatePseudoMovesBoth(const Position & position, MoveList & moves);

		// Make sure these remain inlined to prevent overhead asociated with parameters

		static inline void generatePawnMovesWhite(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generatePawnMovesBlack(const Position & position, BoardSquare square, MoveList & moves);
		static inline void generateRookMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateBishopMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateKnightMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);
		static inline void generateQueenMoves(const Position & position, BoardSquare square, bool isWhite, MoveList & moves);

	};
} // namespace forge

#include "MoveGeneratorInline.h"	// Includes inline definitions