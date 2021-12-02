#include "MCTS_Solver.h"

#include "GameState.h"

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
	
	MovePositionPair MCTS_Solver::solve(const Position& position)
	{
		MovePositionPair bestMove;

		return bestMove;
	}
} // namespace forge