#include "Rollout.h"

#include "forge/controllers/RandomSolver.h"
#include <forge/core/game_history.h>
#include <forge/core/GameState.h>

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t Rollout::eval(const Position& pos)
		{
			RandomSolver rsolver;	// makes random moves
			game_history history;	// records moves to determine draws
			GameState gstate;		// determines state of game: win, loss, draw, continue

			// Start with current position
			history.emplace_back(Move{}, pos);

			// Determine current state. 
			// We might already be at a terminal node.
			gstate(history);

			// Repeat until reaching a terminal node
			while (gstate.isGameOn()) {
				// Alias current position
				const Position& curr = history.current().position;

				// Pick random move
				MovePositionPair next = rsolver.getMove(curr);
				history.emplace_back(next);

				// Determine state
				gstate(history);
			}

			// *** By now we will have reached a terminal node (win, loss, or draw) ***

			// Return the value of the result
			return gstate.getValue();
		}
	} // namespace heuristic
} // namespace forge
