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
				cout << guten::color::push() << guten::color::green 
					<< "position fen " << pos.toFEN() << endl
					<< guten::color::pop();
				out << "position fen " << pos.toFEN() << endl;

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

						cout << guten::color::push() << guten::color::red 
								<< move << guten::color::pop() << endl;

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

			void stockfishAndForge()
			{
				boost::process::child stockfish;
				bp::opstream sfOut;
				bp::ipstream sfIn;

				stack<forge::Position> frontier;

				forge::Position pos;	// Start with opening position
				pos.reset();

				frontier.push(pos);

				while (frontier.size()) {
					pos = frontier.top();
					frontier.pop();

					// 0.) --- Break if exit condition is reached ---
					// We don't know what exit conditions we want yet

					// 1.) --- Call StockFish's movegen on Position 'pos' ---
					vector<Move> sfMoves = moveGenLC0(stockfish, sfOut, sfIn, pos);
					cout << guten::color::magenta << sfMoves.size() << " moves generated by stockfish\n";

					// 2.) --- Convert Moves to Positions relative to 'pos' ---
					// 3.) --- Push Positions to a set ---
					std::set<forge::Position> sfPositions = movesToPositions(pos, sfMoves);

					// 4.) --- Call Forge's movegen on Position 'pos' ---
					MoveGenerator2 movegen;
					MoveList forgeMoveList = movegen.generate(pos);
					set<forge::Position> forgePositions;
					transform(
						forgeMoveList.begin(), forgeMoveList.end(),
						inserter(forgePositions, forgePositions.end()),
						[](const MovePositionPair& pair) { return pair.position; });

					// 5.) --- Match Positions between SF's and forge's Positions ---
					// Organize into 3 sets:
					//	- matches - moves generated by both SF and forge
					//	- misses - moves generated by SF alone (forge missed these)
					//	- faults - moves generated by forge alone (forge should not have generated theses)
					std::set<forge::Position> matches;
					std::set<forge::Position> misses;
					std::set<forge::Position> faults;

					matchPositions(
						sfPositions,
						forgePositions,
						matches,
						misses,
						faults);

					// 6.) --- Push Positions from SF into frontier so that we can search them later ---
					for (const auto& pos : sfPositions) {
						frontier.push(pos);
					}

					// 7.) --- Show results ---
					showResults(pos, matches, misses, faults);

					cout << "Press any key...";
					cin.get();
				} // while(
			} // stockfishAndForge()
		} // namespace movegen
	} // namespace test
} // namespace forge