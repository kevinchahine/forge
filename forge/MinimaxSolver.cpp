#include "MinimaxSolver.h"

#include <typeinfo>

using namespace std;

namespace forge
{
	void MinimaxSolver::reset()
	{
		m_nodeTree.reset();
	}

	Move MinimaxSolver::getMove(const Position & position)
	{
		Move bestMove = solve(position);

		return bestMove;
	}

	string MinimaxSolver::getName()
	{
		return typeid(*this).name();
	}

	Move MinimaxSolver::solve(const Position & position)
	{
		m_searchMonitor.start();

		m_nodeTree.reset();

		m_nodeTree.position() = position;	// Copy position into root of node tree

		Node::iterator it = m_nodeTree.begin();
		it.setDepthLimit(8);

		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Get current position to evaluate ---
			Position & pos = (*it).position();

			// --- 2.) Make sure position is valid ---
			// Make sure movers King is not left in check ---
			// ex: If white's moving, then its King should not be in check

			// --- 3.) Check game state (is this a terminal node) ---
			// TODO: GAME WILL probably crash when reaching a terminal node since no valid
			//	moves will be generated
			// TODO: WE REALLY NEED TO DO THIS NEXT

			// --- 4.) Evaluate this position ---
			// We only want to evaluate leaf nodes
			if (it.isLeafNode()) {
				(*it).fitness() = heuristicPtr->eval(pos);
			}

			// --- 5.) Move to next node ---
			++it;

			// Did we reached the root node?
			if (it == m_nodeTree.end()) {
				break;
			}
		}

		//cout << m_nodeTree.bestMove() << '\n';

		return m_nodeTree.bestMove();
	}
} // namespace forge