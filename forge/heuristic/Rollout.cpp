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
			return eval(pos, true);
		}
		
		heuristic_t Rollout::eval(const Position& pos, bool maximizeWhite)
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
			return gstate.getValue(maximizeWhite);
		}
		std::vector<heuristic_t> Rollout::eval(const std::vector<const Position *> & positions, bool whiteIsSearching) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << ": method not implemented" << endl;

			return std::vector<heuristic_t>();
		}
	} // namespace heuristic
} // namespace forge
