#include "DatasetGenerator.h"
#include "UciEngine.h"
#include "RandomSolver.h"

#include <unordered_map>

using namespace std;
using namespace forge::uci;

namespace forge
{
	void DatasetGenerator::run()
	{
		Position pos;
		int eval;

		unordered_map<Position, int> evals;

		RandomSolver solver;

		uci::UciEngine stockfish;

		stockfish.launch();

		while (true) {
			cout << pos << endl;
			eval = stockfish.eval(pos);

			MovePositionPair pair = solver.getMove(pos);

			if (pair.move.isValid()) {
				pos = pair.position;
			}
			else {
				cout << "Invalid";
				pos = Position{};
			}

			cout << "Eval is " << eval << " centipawns" << endl;

			evals.emplace(pos, eval);
		}

		for (const auto& elem : evals) {
			cout << elem.first.toFEN() << "\t\t" << elem.second << endl;
		}
	}
} // namespace forge