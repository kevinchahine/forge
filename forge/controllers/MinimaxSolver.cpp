#include "forge/controllers/MinimaxSolver.h"

#include "forge/game/GameState.h"

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

		m_nodeTree = MiniMaxNode{};
		m_nodeTree.position() = position;	// Copy position into root of node tree

		MiniMaxNode::iterator it = m_nodeTree.begin();
		it.setDepthLimit(4);

		while (
			it != m_nodeTree.end() &&
			m_searchMonitor.exitConditionReached() == false) {

			// --- 1.) Get current position to evaluate ---
			Position& pos = (*it).position();

			// --- 3.) Check game state (is this a terminal node) ---
			// TODO: WE REALLY NEED TO DO THIS NEXT
			// Could this be done more easily in isLeafNode() see below vvv
			GameState state;
			state(*it);

			// --- 4.) Evaluate this position ---
			// We only want to evaluate leaf nodes
			// Is the game finished?
			if (state.isGameOver()) {
				// --- Game Is Finished ---
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
			}
			else if (it.isLeafNode()) {
				(*it).fitness() = heuristicPtr()->eval(pos);
			}
			else {
				// Game is still going
			}

			// --- 5.) Move to next node ---
			++it;

			m_searchMonitor.nodeCount = it.getNodeCount();
			m_searchMonitor.plyCount = std::max(it.getDepth(), m_searchMonitor.plyCount);
		} // end while (

		// *** Now the search is complete. Hopefully we found a best move. ***
		// *** The only times we won't find a best move is when there are no children ***
		//		moves to choose from. Meaning no legal moves. 

		cout << termcolor::bright_blue << "Best Move Ptr = " << m_nodeTree.bestMovePtr() << '\n';
		cout << termcolor::green << "Nodes searched: " << m_searchMonitor.nodeCount << '\t'
			<< "search time: " << chrono::duration_cast<chrono::milliseconds>(m_searchMonitor.searchTime.elapsed()).count()/1000.0 << " sec\t"
			<< m_searchMonitor.nodesPerSecond() << " nodes/sec\n";
		
		m_searchMonitor.stop();

		// --- Determine the best move ---

		MovePositionPair solution;

		// Was a best move found?
		if (m_nodeTree.bestMovePtr() != nullptr) {
			// Yes a best move was found.
			solution = MovePositionPair{
				m_nodeTree.bestMovePtr()->move(),
				m_nodeTree.bestMovePtr()->position() };
		}
		else {
			// TODO: Complain gracefully. Something went wrong. We couldn't find a solution
#ifdef _DEBUG
			cout << termcolor::push
				<< termcolor::on_red
				<< "Error: " << __FUNCTION__ << " line " << __LINE__
				<< " No solution was found.\n"
				<< termcolor::pop;
#endif // _DEBUG

			solution = MovePositionPair{
				m_nodeTree.move(),
				m_nodeTree.position()
			};
		}

		return solution;
	}
} // namespace forge