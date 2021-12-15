#include "MCTS_Solver.h"

#include "GameState.h"
#include "RandomSolver.h"	// Used in rollout
#include "GameHistory.h"	// Used in rollout
#include "GameState.h"		// Used in rollout

using namespace std;

namespace forge
{
	void MCTS_Solver::reset()
	{
		m_nodeTree.reset();
	}

	MovePositionPair MCTS_Solver::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);

		return bestMove;
	}

	void MCTS_Solver::traverse()
	{
		it.goToSelectedChild();
	}

	void MCTS_Solver::expand()
	{
		(*it).expand();

		it.goToFirstChild();
	}

	int MCTS_Solver::rollout()
	{
		RandomSolver rsolver;	// makes random moves
		GameHistory history;	// records moves to determine draws
		GameState gstate;		// determines state of game: win, loss, draw, continue

		// Start with current position
		history.emplace_back((*it).position());

		// Determine current state. 
		// We might already be at a terminal node.
		gstate(history);
		
		// Repeat until reaching a terminal node
		while (gstate.isGameOn()) {
			// Alias current position
			const Position& curr = history.current();

			// Pick random move
			MovePositionPair next = rsolver.getMove(curr);
			history.emplace_back(next.position);
			//cout << "Rollout: " << endl;
			//next.position.board().printMini();
			// Determine state
			gstate(history);
		}

		// *** By now we will have reached a terminal node ***

		// Return the value of the result
		return gstate.getValue();
	}

	void MCTS_Solver::backpropagate(int rolloutResult)
	{
		MCTS_Node::iterator backIt = it;
		
		// Repeat until we reach the root node. (Which has no parent)
		while (backIt.parentExists()) {
			MCTS_Node& curr = (*backIt);

			curr.update(rolloutResult);

			backIt.goToParent();
		};

		// Do once more for root node
		m_nodeTree.updateRoot(rolloutResult);
	}

	MovePositionPair MCTS_Solver::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.begin();

		bestIt.goToBestChild();

		MovePositionPair solution{
			(*bestIt).move(),
			(*bestIt).position()
		};

		return solution;
	}
	
	MovePositionPair MCTS_Solver::solve(const Position& position)
	{
		m_searchMonitor = SearchMonitor{};

		m_searchMonitor.timer.pause();
		m_searchMonitor.timer.expires_from_now(chrono::seconds(4));
		//m_searchMonitor.nodeLimit = 10000;
		m_searchMonitor.start();

		m_nodeTree = MCTS_Node{};
		m_nodeTree.position() = position;	// Copy position into root of node tree

		it = m_nodeTree.begin();

		while (m_searchMonitor.exitConditionReached() == false) {
			// --- 1.) Get current position to evaluate --- 
			Position& pos = (*it).position();

			// --- 2.) Tree Traversal ---
			// Has this node been expanded?
			if ((*it).isLeaf()) {
				// Not yet, lets expand it and see if it has kids.
				// Either this node is:
				//	- an intermediate node with children
				//	- a terminal node with no children
				
				// Have we explored this node yet?
				if ((*it).nGamesVisited() == 0) {
					// No, this node is unexplored (meaning its not expanded)
					// --- 3.) Node Expansion ---
					expand();
				}

				// --- 4.) Roll Out ---
				int result = rollout();

				// --- 5.) Backpropagation ---
				backpropagate(result);

				it = m_nodeTree.begin();
			}
			else {
				// Yes this node does have kids. 
				// Simply select a child and continue traversing.
				
				// *** Here we can safely assume that the node has kids ***
				traverse();

				m_searchMonitor.nodeCount++;
			}
		}

		m_searchMonitor.stop();

		return selectBestMove();
	}
} // namespace forge