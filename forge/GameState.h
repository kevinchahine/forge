#pragma once

#include <iostream>
#include <bitset>

#include "GameHistory.h"
#include "Position.h"
#include "Node.h"

namespace forge
{
	// Purpose: Represents a game state and calculates the game state of a Position.
	//	To calculate the game state of a Position, the Position history is needed.
	// Game States:
	//	- continue
	//		- whites turn
	//		- blacks turn
	//	- white wins
	//		- by resignation
	//		- by timeout (with sufficient material)
	//		- by checkmate
	//	- black wins
	//		- by resignation
	//		- by timeout (with sufficient material)
	//		- by checkmate
	//	- draw
	//		- by agreement
	//		- by repetition (the same Position has been reached 3 times)
	//		- by timeout (with insufficient material)
	//		- by 50 move rule
	//		- by insufficient material (even with time on the clock)
	// *** Draws by insufficient material follow USCF rules
	class GameState
	{
	public:
		void reset() { (*this) = GameState(); }

		// Calculates and sets game state
		void operator()(Node & node);
		void operator()(const GameHistory & history);
		//void operator()(const ChessMatch & match);	// TODO:

		bool isGameOver() const { return state != STATE::CONTINUE; }
		bool isGameOn() const { return state == STATE::CONTINUE; }

		friend std::ostream & operator<<(std::ostream & os, const GameState & gs);

	private:
		// Calculates state of game as in: white wins, black wins, draw or continue.
		// params:
		//	- nLegalMoves - Number of legal moves at currPos
		//	- currPos - current position game
		//	- drawByRepetition - function that calculates a draw by repetition.
		//		Function will need to contain in some way all the moves or positions
		//		that lead to currPos, and return true if currPos has been found in 
		//		game history atleast 3 times (actually 2 times not counting 'currPos'
		//		itself.
		void calcGameState(int nLegalMoves, const Position & currPos, std::function<bool()> && drawByRepetition);

		static bool isDrawByRepetition(const Node & node);

		static bool isDrawByRepetition(const GameHistory & history);

		static bool isInsufficientMaterial(const Board & board);

	public:
		enum class PLAYER : bool {
			WHITE, BLACK
		} player;

		enum class STATE : uint8_t {
			CONTINUE, WIN, DRAW
			// No lose because a win for one player is a loss for the other
		} state;

		enum class REASON : uint8_t {
			NOTHING,			// (CONTINUE only) For when game is still playing.
			AGREEMENT,			// (DRAW only) Where both players agree on a draw 
			RESIGNATION,		// (WIN only) where one player resigns
			CHECKMATE,			// (WIN only)
			STALEMATE,			// (DRAW only) No pieces can move but King is not being attacked
			REPETITION,			// (DRAW only) 3 fold repetition
			FIFTY_MOVE_RULE,	// (DRAW only) 50 moves have passed without captures or pawn moves
			INSUFFICIENT_MATERIAL_ONLY,	// (DRAW only) Neither players have enough pieces to administer checkmate
			TIMEOUT_ONLY,		// (WINS only) One player ran out of time while the other has sufficient material to administer checkmate
			TIMEOUT_AND_INSUFFICIENT_MATERIAL,	// (DRAW only) One player ran out of time while other doesn't have enough material to administer checkmate
		} reason;
	};
} // namespace forge