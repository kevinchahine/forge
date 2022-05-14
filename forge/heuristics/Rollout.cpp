#include "Rollout.h"

#include "../RandomSolver.h"
#include "../GameHistory.h"
#include "../GameState.h"

using namespace std;

namespace forge
{
	namespace heuristics
	{
		heuristic_t Rollout::eval(const Position& pos)
		{
			RandomSolver rsolver;	// makes random moves
			GameHistory history;	// records moves to determine draws
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
	} // namespace heuristics
} // namespace forge
