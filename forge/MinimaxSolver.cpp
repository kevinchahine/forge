#include "MinimaxSolver.h"

#include "GameState.h"

#include <limits>

using namespace std;

namespace forge
{
	void MinimaxSolver::reset()
	{
		m_nodeTree.reset();
	}

	MovePositionPair MinimaxSolver::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);

		return bestMove;
	}

	MovePositionPair MinimaxSolver::solve(const Position& position)
	{
		m_searchMonitor.timer.expires_from_now(chrono::hours(1));
		m_searchMonitor.start();

		m_nodeTree.reset();
		m_nodeTree.position() = position;	// Copy position into root of node tree

		MiniMaxNode::iterator it = m_nodeTree.begin();
		it.setDepthLimit(4);

		while (
			it != m_nodeTree.end() &&
			m_searchMonitor.exitConditionReached() == false) {

			// --- 1.) Get current position to evaluate ---
			Position& pos = (*it).position();

			(*it).expand();
			// --- 3.) Check game state (is this a terminal node) ---
			// TODO: WE REALLY NEED TO DO THIS NEXT
			// Could this be done more easily in isLeafNode() see below vvv
			GameState state;
			state(*it);

			// --- 4.) Evaluate this position ---
			// We only want to evaluate leaf nodes
			if (state.isGameOver()) {
				// Do we have a winner?
				if (state.state == GameState::STATE::WIN) {
					// Yes we have a winner!!!
					(*it).fitness() = (
						state.player == GameState::PLAYER::WHITE ?
						std::numeric_limits<heuristic_t>::max() :		// WHITE won
						std::numeric_limits<heuristic_t>::lowest());	// BLACK won
				}
				else if (state.state == GameState::STATE::DRAW) {
					(*it).fitness() = 0;								// DRAW
				}

				// TODO: I don't know about this.
				///(*it).prune();	// make sure no children get evaluated
			}
			else if (it.isLeafNode()) {
				(*it).fitness() = heuristicPtr()->eval(pos);
			}
			else {
			}

			// --- 5.) Move to next node ---
			++it;

			m_searchMonitor.nodeCount = it.getNodeCount();
			m_searchMonitor.plyCount = std::max(it.getDepth(), m_searchMonitor.plyCount);
		} // end while (

		// *** Now the search is complete. Hopefully we found a best move. ***
		// *** The only times we won't find a best move is when there are no children ***
		//		moves to choose from. Meaning no legal moves. 

		cout << guten::color::lightblue << "Best Move Ptr = " << m_nodeTree.bestMovePtr() << '\n';

		// Was a best move found?
		if (m_nodeTree.bestMovePtr() != nullptr) {
			// Yes a best move was found.
			return MovePositionPair{
				m_nodeTree.bestMovePtr()->move(),
				m_nodeTree.bestMovePtr()->position() };
		}
		else {
			// TODO: Complain gracefully. Something went wrong. We couldn't find a solution
#ifdef _DEBUG
			cout << guten::color::push()
				<< guten::color::red.inverted()
				<< "Error: " << __FUNCTION__ << " line " << __LINE__
				<< " No solution was found.\n"
				<< guten::color::pop();
#endif // _DEBUG

			return MovePositionPair{
				m_nodeTree.move(),
				m_nodeTree.position()
			};
		}

		m_searchMonitor.stop();
	}
} // namespace forge