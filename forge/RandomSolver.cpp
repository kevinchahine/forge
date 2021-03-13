#include "RandomSolver.h"
#include "MoveGenerator.h"

#include <random>
#include <chrono>

using namespace std;

namespace forge
{
	Move RandomSolver::getMove(const Position & position)
	{
		const auto validMoves = MoveGenerator::generatePseudoMoves(position);

		if (validMoves.size() == 0) {
			cout << "No valid moves were generated\n";
			cin.get();
		}

		uniform_int_distribution<size_t> dist(0, validMoves.size() - 1);

		return validMoves.at(dist(m_randomEngine)).move;
	}
} // namespace forge