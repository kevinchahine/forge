#include "MinimaxSolver.h"

using namespace std;

namespace forge
{
	void MinimaxSolver::reset()
	{
		m_nodeTree.reset();
	}

	Move MinimaxSolver::getMove(const Position & position)
	{
		if (m_bestLine.empty()) {
			m_bestLine = solve(position);
		}
		
		if (m_bestLine.size()) {
			MovePositionPair pair = m_bestLine.front();	// must be a copy not reference
			
			m_bestLine.pop_front();

			return pair.move;
		}
		else {
			throw std::exception("No valid moves");
		}
	}

	Line MinimaxSolver::solve(const Position & position)
	{
		Line bestLine;

		m_searchMonitor.start();
		
		m_nodeTree.position() = position;	// Copy position into root of node tree

		Node::iterator it = m_nodeTree.begin();
		it.setDepthLimit(3);

		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Get current position to evaluate ---
			Position & pos = (*it).position();

			// --- 2.) Make sure position is valid ---
			// Make sure movers King is not left in check ---
			// ex: If white's moving, then its King should not be in check

			// --- 3.) Check game state (is this a terminal node) ---
			// TODO: GAME WILL probably crash when reaching a terminal node since no valid
			//	moves will be generated

			// --- 4.) Evaluate this position ---
			// We only want to evaluate leaf nodes
			heuristic_t eval = 0;
			if (it.isLeafNode()) {
				eval = heuristicPtr->eval(pos);

				(*it).fitness() = eval;
			}

			// --- 5.) Move to next node ---
			++it;

			// Did we reached the root node?
			if (it == m_nodeTree.end()) {
				break;
			}
		}

		return bestLine;
	}
} // namespace forge