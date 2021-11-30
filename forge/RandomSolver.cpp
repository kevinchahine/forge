#include "RandomSolver.h"
#include "MoveGenerator2.h"

#include <random>
#include <chrono>

using namespace std;

namespace forge
{
	MovePositionPair RandomSolver::getMove(const Position & position)
	{
		MoveGenerator2 movegen;
		const auto validMoves = movegen.generate(position);

		if (validMoves.size() == 0) {
			cout << "No valid moves were generated\n";
			cin.get();
		}

		uniform_int_distribution<size_t> dist(0, validMoves.size() - 1);

		return validMoves.at(dist(m_randomEngine));
	}
} // namespace forge