#include "TestMoveGen.h"
#include "Position.h"
#include "MoveGenerator2.h"
#include "MoveList.h"

#include <Guten/Color.h>
#include <Guten/GridView.h>

#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>
#include <boost/process/pipe.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <set>
#include <thread>
#include <chrono>

using namespace std;
namespace fs = boost::filesystem;
namespace bp = boost::process;

// TODO: still need to define for Position
// - hash<>
// - operator<()

namespace forge
{
	namespace test
	{
		namespace movegen
		{
			void runChessEngine(bp::child& chessEngine, bp::opstream& out, bp::ipstream& in)
			{
				fs::path chessEnginePath{ R"dil(C:\Program Files\stockfish_13_win_x64_bmi2\stockfish_13_win_x64_bmi2.exe)dil" };

				if (!fs::exists(chessEnginePath)) {
					cout << chessEnginePath << " does not exist" << endl;
					cin.get();
				}
				else {
					chessEngine = bp::child{ chessEnginePath, bp::std_in < out, bp::std_out > in };

					if (chessEngine.running()) {
						cout << chessEnginePath << " is running" << endl;
					}
					else {
						cout << "Error: " << chessEnginePath << " is not running" << endl;
						cin.get();
					}
			
					// --- Set engine to UCI mode ---
					cout << guten::color::push() << guten::color::green
						<< "uci" << guten::color::pop() << endl;

					out << "uci" << endl;

					cout << guten::color::push() << guten::color::brown;
					while (chessEngine.running()) {
						string line;
						getline(in, line);
						cout << line << endl;

						if (boost::algorithm::starts_with(line, "uciok")) {
							break;
						}
					}
					cout << guten::color::pop();

					cout << guten::color::push() << guten::color::green
						<< "setoption name MultiPV value 500" << endl
						<< "ucinewgame" << endl
						<< guten::color::pop();

					out << "setoption name MultiPV value 500" << endl
						<< "ucinewgame" << endl;
				}
			}

			std::vector<forge::Move> moveGenLC0(
				bp::child& chessEngine, 
				bp::opstream& out, 
				bp::ipstream& in, 
				const forge::Position & pos)
			{
				std::vector<forge::Move> lc0Moves;
				
				if (!chessEngine.running()) {
					runChessEngine(chessEngine, out, in);
				}

				// --- Set Position --- 


				// --- Get Legal Moves ---
				cout << guten::color::push() << guten::color::green
					<< "go depth 1" << endl
					<< guten::color::pop();

				out << "go depth 1" << endl;

				cout << guten::color::push() << guten::color::brown;
					
				string line;
				getline(in, line);	// ignore 1st line
				cout << line << endl;

				while (chessEngine.running()) {
					getline(in, line);
					cout << line << endl;

					if (boost::algorithm::starts_with(line, "bestmove")) {
						break;
					}
					else {

						size_t pvPos = line.find("pv", 76);
						size_t movePos = pvPos + 3;

						string move = line.substr(movePos, movePos + 4);

						cout << guten::color::push() << guten::color::red << move << guten::color::pop() << endl;

						lc0Moves.emplace_back(std::move(move));
					}
				}
				cout << guten::color::pop();

				return lc0Moves;
			}

			std::set<forge::Position> movesToPositions(
				const forge::Position& base,
				const std::vector<Move>& moves)
			{
				std::set<forge::Position> posSet;

				for (const auto& move : moves) {
					forge::Position pos = base;

					pos.move<pieces::Piece>(move);

					posSet.insert(pos);
				}

				cout << guten::color::lightmagenta << "posSet.size() " << posSet.size() << endl;

				return posSet;
			}

			void matchPositions(
				const std::set<forge::Position>& lc0Poss,
				const std::set<forge::Position>& forgePoss,
				std::set<forge::Position>& matches,
				std::set<forge::Position>& misses,
				std::set<forge::Position>& faults)
			{
				// --- Find Matches (Found in both LC0 and Forge) ---
				set_intersection(
					lc0Poss.begin(), lc0Poss.end(),
					forgePoss.begin(), forgePoss.end(),
					inserter(matches, matches.end()));

				// --- Find Misses (unique to LC0) ---
				set_difference(
					lc0Poss.begin(), lc0Poss.end(),
					forgePoss.begin(), forgePoss.end(),
					inserter(misses, misses.end()));

				// --- Find Faults (unique to Forge) ---
				set_difference(
					forgePoss.begin(), forgePoss.end(),
					lc0Poss.begin(), lc0Poss.end(),
					inserter(misses, misses.end()));
			}

			guten::grids::GridView makeGridView(std::set<forge::Position>& poss)
			{
				guten::grids::GridView grid;
				grid.setGridCols(10);

				for (const auto& pos : poss) {
					grid.push(pos.board().getMiniBoard());
				}

				return grid;
			}

			void showResults(
				const forge::Position& base,
				std::set<forge::Position>& matches,
				std::set<forge::Position>& missed,
				std::set<forge::Position>& faults)
			{
				base.board().print();

				guten::grids::GridView matchGrid = makeGridView(matches);
				guten::grids::GridView missedGrid = makeGridView(missed);
				guten::grids::GridView faultsGrid = makeGridView(faults);

				cout << endl << guten::color::push() << guten::color::green
					<< "=== Matches (Good) ===" << guten::color::pop() << endl;
				matchGrid.toMatrix().print();

				cout << endl << guten::color::push() << guten::color::yellow
					<< "=== Missed (Getting There) ===" << guten::color::pop() << endl;
				missedGrid.toMatrix().print();

				cout << endl << guten::color::push() << guten::color::red
					<< "=== Faults (Bad We should not have generated these) ===" << guten::color::pop() << endl;
				faultsGrid.toMatrix().print();
			}

			void lc0AndForge()
			{
				boost::process::child lc0;
				bp::opstream lc0out;
				bp::ipstream lc0in;

				stack<forge::Position> frontier;

				forge::Position pos;
				pos.reset();

				frontier.push(pos);

				while (frontier.size()) {
					forge::Position base = frontier.top();
					frontier.pop();

					// 0.) --- Break if exit condition is reached ---
					// We don't know what exit conditions we want

					// 1.) --- Call LC0's movegen on Position pos ---
					vector<Move> lc0Moves = moveGenLC0(lc0, lc0out, lc0in, base);
					cout << guten::color::magenta << lc0Moves.size() << " moves generated by stockfish\n";

					// 2.) --- Convert Moves to Positions relative to pos ---
					// 3.) --- Push Positions to a set ---
					std::set<forge::Position> lc0Poss = movesToPositions(base, lc0Moves);

					// 4.) --- Call forge's movegen on Position pos ---
					MoveGenerator2 movegen;
					MoveList forgePoss = movegen.generate(pos);
					set<forge::Position> lc0Set;
					transform(
						forgePoss.begin(), forgePoss.end(),
						inserter(lc0Set, lc0Set.end()),
						[](const MovePositionPair& pair) { return pair.position; });

					// 5.) --- Match Positions between LC0's and forge's Positions ---
					// Organize into 3 sets:
					//	- matches - moves generated by both LC0 and forge
					//	- misses - moves generated by LC0 alone (forge missed these)
					//	- faults - moves generated by forge alone (forge should not have generated theses)
					std::set<forge::Position> matches;
					std::set<forge::Position> misses;
					std::set<forge::Position> faults;

					matchPositions(
						lc0Poss,
						lc0Set,
						matches,
						misses,
						faults);

					// 6.) --- Push Positions from LC0 into frontier ---
					for (const auto& pos : lc0Poss) {
						frontier.push(pos);
					}

					// 7.) --- Show results ---
					showResults(base, matches, misses, faults);

					cout << "Press any key...";
					cin.get();
				} // while(
			} // lc0AndForge()
		} // namespace movegen
	} // namespace test
} // namespace forge