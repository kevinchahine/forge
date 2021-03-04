#pragma once

#include <iostream>
#include <bitset>

namespace forge
{
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
	//		- by insufficient material
	class GameState
	{
	public:

		friend std::ostream & operator<<(std::ostream & os, const GameState & gs);

	public:
		// MSB			LSB
		// abbc'ccdd
		//	a: player
		//		- 0=white 
		//		- 1=black
		//	bb:	game state
		//		- 00=continue 
		//		- 01=win 
		//		- 10=draw 
		//		- 11=reserved
		//	ccc: reason 1 for game state
		//		- 000=nothing (mostly for when game is still playing)
		//		- 001=resignation or agreement (mostly for draws and resignations)
		//		- 011=checkmate
		//		- 100=cant move (no pieces can move but not in check)
		//		- 101=repetition (3 fold repetition)
		//		- 110=50 move rule
		//	dd: reason 2 for game state
		//		- 01=insufficient material
		//		- 10=timeout 

		enum class PLAYER : bool {
			WHITE, BLACK
		} player;

		enum class STATE {
			CONTINUE, WIN, DRAW
		} state;

		enum class REASON {
			NOTHING,
			AGREEMENT,
			CHECKMATE,
			CANT_MOVE,
			REPETITION,
			FIFTY_MOVE_RULE,
			INSUFFICIENT_MATERIAL_ONLY,
			TIMEOUT_ONLY,
			TIMEOUT_AND_INSUFFICIENT_MATERIAL,
		} reason;
	};
} // namespace forge