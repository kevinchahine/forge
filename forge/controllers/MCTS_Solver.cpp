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
		m_searchMonitor.timer.expires_from_now(chrono::seconds(4));
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
						eval = 1'000 * gstate.getValue(maximizeWhite);	// count a win a 10 pawns
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
				if (curr.hasChildren()) {
					curr.toBestUCB(maximizeWhite);
				}
				else {
					// We reached a terminal node (expended but without children).
					// The only thing we can do here is go back to the root and continue the search.
					curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.
					
					// TODO: Optimize: This code is very inefficient. When all nodes in a subtree are terminal 
					//	(win/lose/draw no children even when expanded)
					//	continuing to search this subtree will result in no added computation. 
					//	A possible optimization is to mark each node as being fully searched when all its
					//	leaf nodes are terminal.
					//	Or we can prune subtrees when they are fully searched. (Might not be the best option
					//	for multithreading. Also we are deleting data we can use in the next search.)
					if (m_searchMonitor.exitConditionReached()) {
						m_searchMonitor.stop();	// stop the clock so we can record exact search time.
						break;
					}
				}
			}
		}

		return selectBestMove();
	}
} // namespace forge