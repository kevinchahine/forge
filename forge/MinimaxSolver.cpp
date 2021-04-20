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

	MovePositionPair MinimaxSolver::getMove(const Position & position)
	{
		MovePositionPair bestMove = solve(position);

		return bestMove;
	}

	string MinimaxSolver::getName()
	{
		return "Minimax";
	}

	MovePositionPair MinimaxSolver::solve(const Position & position)
	{
		m_searchMonitor.start();

		m_nodeTree.reset();

		m_nodeTree.position() = position;	// Copy position into root of node tree

		Node::iterator it = m_nodeTree.begin();
		it.setDepthLimit(2);

		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Get current position to evaluate ---
			if (it == m_nodeTree.end()) {
				cout << "Node tree has reached its END.\n";
				cout << "\tImportant question. Is the best child ptr still valid?\n";
				break;
			}

			Position & pos = (*it).position();
			cout << it.getDepth() << ' ';
			///cout << '.';
			///pos.board().printMini();
			///cin.get();
			// --- 3.) Check game state (is this a terminal node) ---
			// TODO: GAME WILL probably crash when reaching a terminal node since no valid
			//	moves will be generated
			// TODO: WE REALLY NEED TO DO THIS NEXT
			// Could this be done more easily in isLeafNode() see below vvv
			GameState state;
			state(*it);

			// --- 4.) Evaluate this position ---
			// We only want to evaluate leaf nodes
			if (state.isGameOver()) {
				cout << guten::color::brown << "GAME OVER!!!\n";

				if (state.state == GameState::STATE::WIN) {
					(*it).fitness() = (
						state.player == GameState::PLAYER::WHITE ?
						std::numeric_limits<heuristic_t>::max() :		// WHITE won
						std::numeric_limits<heuristic_t>::lowest());	// BLACK won
					cout << guten::color::green << state << '\n';
				}
				else if (state.state == GameState::STATE::DRAW) {
					(*it).fitness() = 0;								// DRAW
					cout << guten::color::blue << state << '\n';
				}

				// TODO: I don't know about this.
				///(*it).prune();	// make sure no children get evaluated
			}
			else if (it.isLeafNode()) {
				(*it).fitness() = heuristicPtr->eval(pos);
				cout << guten::color::push()
					<< guten::color::brown << " Depth limit reached. Eval = "
					<< guten::color::cyan << ' ' << (*it).fitness() << "\n"
					<< guten::color::pop();
			}
			else {
				///cout << guten::color::red << '.';
			}

			// --- 5.) Move to next node ---
			if (it != m_nodeTree.end()) {
				++it;
			}
			// Did we reached the root node?
			else {
#ifdef _DEBUG
				cout << guten::color::cyan << "Root not reached :)\n";
#endif // TODO: Should this ifdef wrap the entire if (it == ..... ?
				break;
			}
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
	}

} // namespace forge