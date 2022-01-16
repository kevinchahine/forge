#include "DatasetGenerator.h"
//#include "UciEngine.h"
//#include "RandomSolver.h"

#include <iomanip>
#include <unordered_map>

using namespace std;
//using namespace forge::uci;

namespace forge
{
	void DatasetGenerator::run()
	{
//		Position pos;
//		int eval;
//
//		unordered_map<Position, int> evals;
//
//		RandomSolver solver;
//
//		uci::UciEngine stockfish;
//
//		stockfish.launch();
//
//		while (true) {
//			MovePositionPair pair = solver.getMove(pos);
//
//			if (pair.move.isValid()) {
//				pos = pair.position;
//			}
//			else {
//				cout << "Invalid";
//				pos = Position{};
//			}
//
//			cout << guten::color::push() << guten::color::red 
//				<< "Evaluating: " << pair 
//				<< guten::color::pop() << endl;
//
//			eval = stockfish.eval(pos);
//
//			cout << guten::color::push() << guten::color::red << setw(30) 
//				<< pos << " Eval is " << eval << " centipawns" 
//				<< guten::color::pop() << endl << endl;
//
//			evals.emplace(pos, eval);
//		}
//
//		for (const auto& elem : evals) {
//			cout << setw(30) << elem.first.toFEN() << elem.second << endl;
//		}
	}
} // namespace forge