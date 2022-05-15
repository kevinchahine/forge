#include "RandomSolver.h"
#include "MoveGenerator2.h"

#include <random>
#include <chrono>

using namespace std;

namespace forge
{
	MovePositionPair RandomSolver::getMove(const Position& position)
	{
		MovePositionPair ret;

		MoveGenerator2 movegen;
		const auto validMoves = movegen.generate(position);

		if (validMoves.size() == 0) {
			//cout << "No valid moves were generated\n";
			//cin.get();
			ret.move.setInvalid();
			return ret;
		}
		else {
			uniform_int_distribution<size_t> dist(0, validMoves.size() - 1);

			return validMoves.at(dist(m_randomEngine));
		}
	}
} // namespace forge