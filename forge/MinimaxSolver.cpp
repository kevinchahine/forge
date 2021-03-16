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

		MovePositionPair pair = m_bestLine.front();

		m_bestLine.pop_front();

		return pair.move;
	}

	Line MinimaxSolver::solve(const Position & position)
	{
		Line bestLine;

		m_searchMonitor.start();

		Node * nodePtr = &m_nodeTree;

		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Generate valid moves ---
			nodePtr->expand();

			// --- 2.) Pick the 1st valid move (assuming it exists) ---
			if (nodePtr->children().empty()) {
				// No valid moves. Its a draw
				// TODO: ???
			}
			else {
				nodePtr = nodePtr->children().front().get();
			}

			// --- 3.) 
		}

		return bestLine;
	}
} // namespace forge