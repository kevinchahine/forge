#include "MCTS_Solver_MT.h"

using namespace std;

namespace forge
{
	MovePositionPair MCTS_Solver_MT::getMove(const Position& position)
	{
		MovePositionPair bestMove = solve(position);

		return bestMove;
	}
	
	MovePositionPair MCTS_Solver_MT::solve(const Position& position)
	{
		// --- 1.) Create Threads ---
		vector<MCTS_Solver> solvers;
		solvers.resize(m_nThreads);

		vector<thread> threads;
		threads.reserve(m_nThreads);

		// --- 2.) Run solver on each thread ---
		for (size_t t = 0; t < m_nThreads; t++) {
			threads.emplace_back(
				&MCTS_Solver::solve, 
				&solvers.at(t),
				position
			);
		}

		for (size_t t = 0; t < m_nThreads; t++) {
			threads.at(t).join();
		}

		threads.clear();

		// --- 3.) Combine trees ---
		this->m_nodeTree.position() = position;
		this->m_nodeTree.expand();
		auto & children = this->nodeTree().children();

		// Go through the decision trees of each thread and combine their children 
		for (const MCTS_Solver& solver : solvers)
		{
			const auto & solverChildren = solver.nodeTree().children();

			for (size_t childIndex = 0; childIndex < children.size(); childIndex++) {
				MCTS_Node & myChild = *children.at(childIndex);
				MCTS_Node& solverChild = *solverChildren.at(childIndex);

				myChild.merge(solverChild);	// accumulate t and n components of node
			}
		}

		// --- 4.) Pick and return best move ---

		return selectBestMove();
	}
} // namespace forge