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

	void MCTS_Solver::expand()
	{
		// --- Create children ---
		(*curr).expand();

		// --- Evaluate 1st child ---
		// Node has not been expanded yet. 
		// We have never generated its children.

		// --- Evaluate 1st Child ---
		if (curr.hasChildren()) {
			curr.toFirstChild();
		}
		else {
			// Stay at curr
		}
	}

	int MCTS_Solver::evaluate()
	{

		return 0;
	}

	MovePositionPair MCTS_Solver::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();

		bool maximize = m_nodeTree.position().moveCounter().isWhitesTurn();

		//bestIt.toSelectedChild(maximize);	// Stochastic selection
		bestIt.toBestChild(maximize);		// Best selection

		MovePositionPair solution{
			(*bestIt).move(),
			(*bestIt).position()
		};

		return solution;
	}

	MovePositionPair MCTS_Solver::solve(const Position& position)
	{
		m_nodeTree.reset();
		m_nodeTree.position() = position;
		curr = m_nodeTree.root();

		while (true) {

			float eval = 0.0f;

			// --- What is the state of this node? ---

			// --- Selection ---
			// --- Expansion ---
			// --- 
			if ((*curr).isFresh()) {
				this->expand();
			}
			else if ((*curr).isExpanded()) {
				bool maximize = (*curr).position().moveCounter().isWhitesTurn();
				curr.toSelectedChild(maximize);
			}
		}

		return selectBestMove();
	}

	//void MCTS_Solver::traverse()
	//{
	//	// Determine whether we should minimize or maximize the selection.
	//	// If thinking player is the same at this ply then we should maximize.
	//	bool thinkingPlayer = m_nodeTree.position().isWhitesTurn();	// Which player is running the search?
	//	bool plyPlayer = (*it).position().isWhitesTurn();			// Which player is at this ply?
	//	bool maximize = (thinkingPlayer == plyPlayer);				// Is it the same player at both? 
	//	
	//	it.toSelectedChild(maximize);
	//}
	
	//void MCTS_Solver::expand()
	//{
	//	(*it).expand();
	//
	//	// Were any children generated?
	//	if ((*it).children().size()) {
	//		// Yes we must be at an intermediate node. Got to 1st child
	//		it.toFirstChild();	// toSelectedChild() will do the same thing.
	//	}
	//	else {
	//		// No children were generated. We are at a terminal node.
	//		// Stay here and let rollout evaluate current node.
	//	}
	//}
	//
	//int MCTS_Solver::rollout()
	//{
	//	Position pos = (*it).position();
	//
	//	// Do we need to rotate the board so that our player always has the white pieces? 
	//	// All evaluation functions work which our player being white.
	//	// If current player is black, we need to rotate and flip the board.
	//	if (m_nodeTree.position().isBlacksTurn()) {
	//		pos.board() = pos.board().rotated();
	//	}
	//
	//	heuristic_t eval = m_heuristicPtr->eval(pos);
	//
	//	return eval;
	//}
	//
	//void MCTS_Solver::backpropagate(int rolloutResult)
	//{
	//	MCTS_Node::iterator backIt = it;
	//	
	//	// Repeat until we reach the root node. (Which has no parent)
	//	while (backIt.hasParent()) {
	//		MCTS_Node& curr = (*backIt);
	//
	//		curr.update(rolloutResult);
	//
	//		backIt.toParent();
	//	};
	//
	//	// Do once more for root node
	//	m_nodeTree.update(rolloutResult);
	//}
	
	//MovePositionPair MCTS_Solver::solve(const Position& position)
	//{
	//	m_searchMonitor = SearchMonitor{};
	//
	//	m_searchMonitor.timer.pause();
	//	m_searchMonitor.timer.expires_from_now(chrono::seconds(4));
	//	//m_searchMonitor.nodeLimit = 10000;
	//	m_searchMonitor.start();
	//
	//	m_nodeTree.reset();
	//	m_nodeTree.position() = position;	// Copy position into root of node tree
	//
	//	it = m_nodeTree.root();				// Start at root
	//
	//	while (m_searchMonitor.exitConditionReached() == false) {
	//		// --- 1.) Get current position to evaluate --- 
	//		Position& pos = (*it).position();
	//
	//		// --- 2.) Tree Traversal ---
	//		// Has this node been expanded?
	//		if ((*it).isLeaf()) {
	//			// Not yet, lets expand it and see if it has kids.
	//			// Either this node is:
	//			//	- an intermediate node with children
	//			//	- a terminal node with no children
	//			
	//			// Have we explored this node yet?
	//			if ((*it).nGamesVisited() == 0) {
	//				// No, this node is unexplored (meaning its not expanded)
	//				// --- 3.) Node Expansion ---
	//				expand();
	//			}
	//
	//			// --- 4.) Roll Out ---
	//			int result = rollout();
	//
	//			// --- 5.) Backpropagation ---
	//			backpropagate(result);
	//
	//			// --- 6.) Start back at the top (root node) ---
	//			it = m_nodeTree.begin();
	//		}
	//		else {
	//			// This node has been expanded so it probably has kids. 
	//			// Make sure it has kids then, simply select a child and
	//			// continue traversing.
	//			// If it doesn't have kids, then we've reached a terminal node
	//			// and its game over.
	//			
	//			// Does it have children?
	//			if ((*it).children().size()) {
	//				// Yes. this node does have children.
	//				// *** Here we can safely assume that the node has kids ***
	//				traverse();
	//
	//				m_searchMonitor.nodeCount++;
	//			}
	//			else {
	//				// No. Expanding this node resulted in no children.
	//				// TODO: MCTS: If we don't have children where do we go?
	//				// Parent? Root? 
	//				it.toParent();		// TODO: THIS MIGHT BE WRONG
	//				//it.goToRoot();	// TODO: THIS MIGHT BE WRONG
	//			}
	//		}
	//	}
	//
	//	m_searchMonitor.stop();
	//
	//	return selectBestMove();
	//}
} // namespace forge