#include "TestMoveGen.h"
#include "Position.h"
#include "MoveGenerator2.h"
#include "MoveList.h"

#include <Guten/Color.h>
#include <Guten/GridView.h>
#include <Guten/DrawFunctions.h>

#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>
#include <boost/process/pipe.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#include <conio.h>	// remove this for Linux compatibility

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <tuple>
#include <string>
#include <sstream>
#include <stack>
#include <set>
#include <vector>

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
					//cout << guten::color::push() << guten::color::green
					//	<< "uci" << guten::color::pop() << endl;

					out << "uci" << endl;

					cout << guten::color::push() << guten::color::brown;
					while (chessEngine.running()) {
						string line;
						getline(in, line);
						//cout << line << endl;

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

			std::vector<forge::Move> moveGenSF(
				bp::child& chessEngine, 
				bp::opstream& out, 
				bp::ipstream& in, 
				const forge::MovePositionPair & movePos)
			{
				const forge::Position& pos = movePos.position;

				std::vector<forge::Move> sfMoves;
				
				if (!chessEngine.running()) {
					cout << guten::color::push() << guten::color::lightgreen
						<< "Running Engine" << guten::color::pop() << endl;

					runChessEngine(chessEngine, out, in);
				}

				// --- Set Position --- 
				//cout << guten::color::push() << guten::color::green 
				//	<< "position fen " << pos.toFEN() << endl
				//	<< guten::color::pop();
				out << "position fen " << pos.toFEN() << endl;

				// --- Get Legal Moves ---
				//cout << guten::color::push() << guten::color::green
				//	<< "go depth 1" << endl
				//	<< guten::color::pop();
				out << "go depth 1" << endl;

				//cout << guten::color::push() << guten::color::brown;
					
				string line;
				getline(in, line);	// ignore 1st line
				///cout << line << endl;

				while (chessEngine.running()) {
					getline(in, line);
					//cout << line << endl;

					if (boost::algorithm::starts_with(line, "bestmove")) {
						break;
					}
					else {

						size_t pvPos = line.find("pv", 76);
						size_t movePos = pvPos + 3;

						string move = line.substr(movePos, movePos + 4);

						///cout << guten::color::push() << guten::color::red 
						///		<< move << guten::color::pop() << endl;

						sfMoves.emplace_back(std::move(move));
					}
				}
				//cout << guten::color::pop();

				return sfMoves;
			}

			std::set<forge::MovePositionPair> movesToPositions(
				const forge::MovePositionPair& base,
				const std::vector<Move>& moves)
			{
				std::set<forge::MovePositionPair> posSet;

				for (const auto& move : moves) {
					forge::MovePositionPair movePos = base;

					movePos.move = move;
					movePos.position.move<pieces::Piece>(move);

					posSet.insert(movePos);
				}

				///cout << guten::color::lightmagenta << "posSet.size() " << posSet.size() << endl;

				return posSet;
			}

			void matchPositions(
				const std::set<forge::MovePositionPair>& sfPoss,
				const std::set<forge::MovePositionPair>& forgePoss,
				std::set<forge::MovePositionPair>& matches,
				std::set<forge::MovePositionPair>& misses,
				std::set<forge::MovePositionPair>& faults)
			{
				// --- Find Matches (Found in both SF and Forge) ---
				set_intersection(
					sfPoss.begin(), sfPoss.end(),
					forgePoss.begin(), forgePoss.end(),
					inserter(matches, matches.end()));

				// --- Find Misses (unique to SF) ---
				set_difference(
					forgePoss.begin(), forgePoss.end(),
					sfPoss.begin(), sfPoss.end(),
					inserter(misses, misses.end()));

				// --- Find Faults (unique to Forge) ---
				set_difference(
					sfPoss.begin(), sfPoss.end(),
					forgePoss.begin(), forgePoss.end(),
					inserter(misses, misses.end()));
			}

			guten::grids::GridView makeGridView(std::set<forge::MovePositionPair>& pairs)
			{
				guten::grids::GridView grid;
				grid.setGridCols(10);

				for (const auto& movePos : pairs) {
					const Move& move = movePos.move;
					const Position& pos = movePos.position;

					guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();
					cb.highlight(move.from().row(), move.from().col());
					cb.highlight(move.to().row(), move.to().col());

					grid.push(cb.drawMini());

					guten::core::Matrix & mat = grid.back();
					guten::draw::putText(mat, move.toLAN(), guten::Point{ 0, 4 }, guten::color::lightcyan);
				}

				return grid;
			}

			void showResults(
				const forge::MovePositionPair& base,
				std::set<forge::MovePositionPair>& matches,
				std::set<forge::MovePositionPair>& missed,
				std::set<forge::MovePositionPair>& faults)
			{
				{
					BoardSquare to = base.move.to();
					BoardSquare from = base.move.from();
					guten::boards::CheckerBoard cb = base.position.board().getCheckerBoard();
					cb.highlight(to.row(), to.col());
					cb.highlight(from.row(), from.col());
					cb.print();

					cout << base.position.toFEN() << endl;
				}

				guten::grids::GridView matchGrid = makeGridView(matches);
				guten::grids::GridView missedGrid = makeGridView(missed);
				guten::grids::GridView faultsGrid = makeGridView(faults);

				cout << endl << guten::color::push() << guten::color::green
					<< "=== Matches (Good) " << matches.size() << " ===" << guten::color::pop() << endl;
				matchGrid.toMatrix().print();
				for (const auto& elem : matches) {
					cout << elem.position.toFEN() << endl;
				}

				cout << endl << guten::color::push() << guten::color::yellow
					<< "=== Missed (Getting There) " << missed.size() << " ===" << guten::color::pop() << endl;
				missedGrid.toMatrix().print();
				for (const auto& elem : missed) {
					cout << elem.position.toFEN() << endl;
				}

				cout << endl << guten::color::push() << guten::color::red
					<< "=== Faults (Bad We should not have generated these) " << faults.size() << " ===" << guten::color::pop() << endl;
				faultsGrid.toMatrix().print();
				for (const auto& elem : faults) {
					cout << elem.position.toFEN() << endl;
				}

				cout << endl;
			}

			void showResults(
				const vector<pair<forge::Position, forge::Move>> & allMisses,
				const vector<pair<forge::Position, forge::Move>> & allFaults)
			{
				cout << endl << guten::color::push() << guten::color::yellow
					<< "=== All Missed (Getting There) " << allMisses.size() << " ===" << guten::color::pop() << endl;
				
				for (const auto& elem : allMisses) {
					guten::boards::CheckerBoard cb = elem.first.board().getCheckerBoard();

					BoardSquare to = elem.second.to();
					BoardSquare from = elem.second.from();

					cb.highlight(to.row(), to.col());
					cb.highlight(from.row(), from.col());

					cb.printMini();
					cout << elem.first.toFEN() << endl;
				}

				cout << endl << guten::color::push() << guten::color::red
					<< "=== Faults (Bad We should not have generated these) " << allFaults.size() << " ===" << guten::color::pop() << endl;
				
				for (const auto& elem : allFaults) {
					guten::boards::CheckerBoard cb = elem.first.board().getCheckerBoard();

					BoardSquare to = elem.second.to();
					BoardSquare from = elem.second.from();

					cb.highlight(to.row(), to.col());
					cb.highlight(from.row(), from.col());

					cb.printMini();
					cout << elem.first.toFEN() << endl;
				}

				cout << endl;
			}

			void stockfishAndForge()
			{
				vector<pair<forge::Position, forge::Move>> allMisses;
				vector<pair<forge::Position, forge::Move>> allFaults;

				boost::process::child stockfish;
				bp::opstream sfOut;
				bp::ipstream sfIn;

				stack<forge::MovePositionPair> frontier;
				const int DEPTH_LIMIT = 5;

				forge::MovePositionPair movePos;	// Start with opening position
				//movePos.position.fromFEN("rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w - - 1 2");
				movePos.position.reset();
				frontier.emplace( movePos );

				while (frontier.size()) {
					movePos = frontier.top();
					frontier.pop();

					// --- Limit Search Depth ---
					if (movePos.position.moveCounter().count >= DEPTH_LIMIT) {
						continue; // Skip this position. It is deeper than we want
					}

					// 0.) --- Break if exit condition is reached ---
					// We don't know what exit conditions we want yet

					// 1.) --- Call StockFish's movegen on Position 'pos' ---
					vector<Move> sfMoves = moveGenSF(stockfish, sfOut, sfIn, movePos);
					// cout << guten::color::magenta << sfMoves.size() << " moves generated by stockfish\n";

					// --- Skip Finished Games ---
					if (sfMoves.empty()) {
						continue;	// Skip this position. It is the last position
					}

					// 2.) --- Convert Moves to Positions relative to 'pos' ---
					// 3.) --- Push Positions to a set ---
					std::set<forge::MovePositionPair> sfPositions = movesToPositions(movePos, sfMoves);

					// 4.) --- Call Forge's movegen on Position 'pos' ---
					MoveGenerator2 movegen;
					MoveList forgeMoveList = movegen.generate(movePos.position);
					set<forge::MovePositionPair> forgePositions;
					copy(
						forgeMoveList.begin(), forgeMoveList.end(),
						inserter(forgePositions, forgePositions.end()));

					// 5.) --- Match Positions between SF's and forge's Positions ---
					// Organize into 3 sets:
					//	- matches - moves generated by both SF and forge
					//	- misses - moves generated by SF alone (forge missed these)
					//	- faults - moves generated by forge alone (forge should not have generated theses)
					std::set<forge::MovePositionPair> matches;
					std::set<forge::MovePositionPair> misses;
					std::set<forge::MovePositionPair> faults;

					matchPositions(
						sfPositions,
						forgePositions,
						matches,
						misses,
						faults);

					// 6.) --- Push Positions from SF into frontier so that we can search them later ---
					for (const auto& pair : sfPositions) {
						frontier.push(pair);
					}

					// 7.) --- Show results ---
					//showResults(movePos, matches, misses, faults);

					for (const auto& elem : misses) {
						allMisses.push_back(std::make_pair(movePos.position, elem.move));
					}
					for (const auto& elem : faults) {
						allMisses.push_back(std::make_pair(movePos.position, elem.move));
					}
					//copy(misses.begin(), misses.end(), inserter(allMisses, allMisses.end()));
					//copy(faults.begin(), faults.end(), inserter(allFaults, allMisses.end()));

					if (_kbhit()) {
						char ch = _getch();

						forge::MovePositionPair pairPH;				// placeholder
						set<forge::MovePositionPair> allMatches;	// placeholder

						cout << guten::color::push() << guten::color::yellow
							<< "========================================================" << endl
							<< "=============== Showing All Results ====================" << endl
							<< "========================================================" << endl
							<< guten::color::pop();

						showResults(allMisses, allFaults);

						cout << "Press any key...";
						cin.get();
					}

					//if (misses.size() || faults.size()) {
					//	cout << "Press any key...";
					//	cin.get();
					//}
				} // while(

				cout << "Done. All Results" << endl;
				forge::MovePositionPair pairPH;				// placeholder
				set<forge::MovePositionPair> allMatches;	// placeholder
				showResults(allMisses, allFaults);
			} // stockfishAndForge()
		} // namespace movegen
	} // namespace test
} // namespace forge