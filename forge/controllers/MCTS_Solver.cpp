#include "MCTS_Solver.h"

#include <forge/core/GameState.h>

using namespace std;

namespace forge
{
	void MCTS_Solver::reset()
	{
		m_nodeTree.reset();
		curr = m_nodeTree.begin();
	}

	MovePositionPair MCTS_Solver::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);

		return bestMove;
	}

	MovePositionPair MCTS_Solver::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();

		bool maximize = m_nodeTree.position().moveCounter().isWhitesTurn();

		//bestIt.toBestUCB(maximize);	// Stochastic selection
		bestIt.toBestAverage(maximize);		// Best selection

		MovePositionPair solution {
			(*bestIt).move(),
			(*bestIt).position()
		};

		return solution;
	}

	MovePositionPair MCTS_Solver::solve(const Position& position)
	{
		// --- Start ---
		m_searchMonitor.timer.expires_from_now(chrono::seconds(1));
		m_searchMonitor.start();

		bool maximizeWhite = position.moveCounter().isWhitesTurn();

		m_nodeTree.reset();
		m_nodeTree.position() = position;
		curr = m_nodeTree.root();
		curr.expand();

		while (true) {

			// --- Selection ---
			if (curr.isLeaf()) {
				heuristic_t eval = 0;

				if ((*curr).isVisited()) {
					curr.expand();

					if (curr.hasChildren()) {
						// TODO: Optimization: Since we will "eventually" evaluate all children 
						// we can optionally evaluate all children at once
						// without significantly changing the algorithms behavior.
						// This can be a good optimization when evaluations are 
						// more efficient in batches.
						heuristic_t eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
					}
					else {
						GameState gstate;
						gstate(*curr);
						eval = 100 * gstate.getValue(maximizeWhite);
					}
				}
				else {
					eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
				}

				// --- Backpropagate ---
				while (curr.isRoot() == false) {
					(*curr).update(eval);
					curr.toParent();
				}
				
				// *** Now curr is at the root ***
				(*curr).update(eval);	// one more time for the root

				// --- Check stopping condition ---
				m_searchMonitor.nodeCount++;
				if (m_searchMonitor.exitConditionReached()) {
					m_searchMonitor.stop();	// stop the clock so we can record exact search time.
					break;
				}
			}
			else {
				// --- Move DOWN the tree ---
				curr.toBestUCB(maximizeWhite);
			}
		}

		return selectBestMove();
	}
} // namespace forge