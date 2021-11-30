#include "MCTS_Solver.h"

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
		MovePositionPair pair;



		return pair;
	}
} // namespace forge