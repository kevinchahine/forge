#include "TestMoveGen.h"
#include "Position.h"
#include "MoveGenerator2.h"
#include "MoveList.h"
#include "StopWatch.h"
#include "globals.h"

#include <Guten/color/Color.h>
#include <Guten/view/GridView.h>
#include <Guten/draw/DrawFunctions.h>

#include <boost/process/child.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>
#include <boost/process/pipe.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _WIN32	// TODO: fIX THIS
#include <conio.h>	// TODO: Cross-Platform: remove this for Linux compatibility
#endif 

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

					// Ignore engine output until "uciok"
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
				const forge::Position & pos)
			{
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

				string line;
				
				while (chessEngine.running()) {
					getline(in, line);
					//cout << line << endl;

					// ignore output that doesn't start with "info depth" and contians "pv"
					if (boost::algorithm::starts_with(line, "bestmove")) {
						// Exit loop. All moves have been printed
						break;
					}
					else if (boost::algorithm::starts_with(line, "info depth")) {
						size_t pvPos = line.find(" pv ", 76);

						// Ignore lines that do not contain "pv"
						if (pvPos != string::npos) {
							size_t movePos = pvPos + 4;

							// TODO: I think we need to substring 5 characters to include promotion char
							size_t movePosEnd = line.find_first_not_of("abcdefgh12345678qrbn", movePos);

							string move = line.substr(movePos, movePosEnd - movePos);
							
							sfMoves.emplace_back(std::move(move));
						}
					}
				}

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

					// TODO: REMOVE:
					if (move.isPartial() || move.isInvalid()) {
						
						base.position.board().printMini();
						cout << "--- " << movePos.move << " ---\n";
						movePos.position.board().printMini();

						//cin.get();
					}

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
					inserter(faults, faults.end()));
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
				//for (const auto& elem : matches) {
				//	cout << elem.position.toFEN() << endl;
				//}

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

				cout << "Press any key...";
				cin.get();
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

					guten::core::Matrix img = cb.drawMini();
					guten::draw::putText(img, elem.second.toLAN(), guten::Point{ 0, 3 }, guten::color::cyan);
					img.print();
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
					guten::core::Matrix img = cb.drawMini();
					guten::draw::putText(img, elem.second.toLAN(), guten::Point{ 0, 3 }, guten::color::cyan);
					img.print();
					cout << elem.first.toFEN() << endl;
				}

				cout << endl;
			}

			forge::Position randomMoves(
				size_t nSkips,						// Number of moves to skip based from currPos
				const forge::Position& currPos,
				boost::process::child& stockfish,
				bp::opstream& sfOut,
				bp::ipstream& sfIn)
			{
				forge::Position pos = currPos;

				for (size_t i = 0; i < nSkips; i++) {
					vector<Move> sfMoves = moveGenSF(stockfish, sfOut, sfIn, pos);

					if (sfMoves.size()) {
						uniform_int_distribution<size_t> dist(0, sfMoves.size() - 1);

						const Move& m = sfMoves.at(dist(g_rand));

						pos.move<pieces::Piece>(m);
					}
					else {
						cout << "No legal moves were generated by stockfish" << endl;
						cout << "Press any key...";
						cin.get();
					}
				}

				return pos;
			}

			void stockfishAndForge()
			{
				vector<pair<forge::Position, forge::Move>> allMisses;
				vector<pair<forge::Position, forge::Move>> allFaults;

				boost::process::child stockfish;
				bp::opstream sfOut;
				bp::ipstream sfIn;

				stack<forge::MovePositionPair> frontier;
				int depthLimit = 200000;
				int nSkips = 0;
				int nodeCount = 0;
				int printedCount = 0;

				// Classify the two search patterns we will use
				enum class SearchPattern {
					DFS,	// Searches left half of decision tree
					RANDOM,	// Searches random vines of the decision tree
				} searchPattern = SearchPattern::DFS;

				forge::StopWatch stopWatch;
				stopWatch.resume();

				forge::MovePositionPair startingPos;
				//startingPos.position.fromFEN(
					//"r2qk2r/2p2ppp/2np4/1p6/1b2n1bP/1PN2N2/P2BKPP1/R2Q3R w kq - 0 1");
					//"K3q3/1P1r2k1/8/pP6/8/8/5b2/3B4 w - - 11 96");
					//"r3kb1r/1pq2ppp/2n1pn2/pBpp4/P2P3N/2PbP3/1P1NKPPP/R1BQ3R w - - 4 -256214754");
				forge::MovePositionPair movePos = startingPos;	// Start with opening position

				// Skip a few moves so that we can test middle and end game positions
				movePos.position = randomMoves(nSkips, movePos.position, stockfish, sfOut, sfIn);

				frontier.emplace( movePos );

				// Raise depthLimit to be relative to starting depth
				depthLimit += frontier.top().position.moveCounter().count;

				while (frontier.size()) {
					movePos = frontier.top();
					frontier.pop();

					// --- Limit Search Depth ---
					if (movePos.position.moveCounter().count >= depthLimit) {
						continue; // Skip this position. It is deeper than we want
					}

					// 0.) --- Break if exit condition is reached ---
					// We don't know what exit conditions we want yet

					// 1.) --- Call StockFish's movegen on Position 'pos' ---
					vector<Move> sfMoves = moveGenSF(stockfish, sfOut, sfIn, movePos.position);
					cout << guten::color::magenta << sfMoves.size() << " moves generated by stockfish\n";
					movePos.position.board().printMini();
					for (const Move& m : sfMoves) {
						cout << m << '\t';
					}
					cout << endl;
					cin.get();

					// --- (Optional: Show progress) ---
					nodeCount += sfMoves.size();
					if (nodeCount - printedCount > 100'000) {
						cout << "Node Count: " << setw(9) << nodeCount 
							<< " Time Elapsed: " << stopWatch 
							<< " misses: " << allMisses.size() 
							<< " faults: " << allFaults.size()
							<< endl;
					
						printedCount = nodeCount;
					}

					// --- Skip Finished Games ---
					if (sfMoves.empty()) {
						cout << guten::color::brown << "Terminal node\n";
						movePos.position.board().printMini();
						continue;	// Skip this position. It is the last position
					}

					// 2.) --- Convert Moves to Positions relative to 'pos' ---
					// 3.) --- Push Positions to a set ---
					std::set<forge::MovePositionPair> sfPositions = movesToPositions(movePos, sfMoves);

					// 4.) --- Call Forge's movegen on Position 'pos' ---
					MoveGenerator2 movegen;
					MoveList forgeMoveList = movegen.generate(movePos.position);
					cout << "Moves generated by forge:";
					forgeMoveList.print();
					cout << endl;

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

					// TODO: wE ALSO need to search for duplicate moves.

					matchPositions(
						sfPositions,
						forgePositions,
						matches,
						misses,
						faults);

					// 6.) --- Push Positions from SF into frontier so that we can search them later ---
					switch (searchPattern) {
					case SearchPattern::DFS:
						for (const auto& pair : sfPositions) {
							frontier.push(pair);
						}
						break;
					case SearchPattern::RANDOM:
					{
						if (sfPositions.empty()) {
							frontier.push(startingPos);
						}
						else {
							uniform_int_distribution<size_t> dist(0, sfPositions.size() - 1);
							size_t index = dist(g_rand);
							auto it = sfPositions.begin();
							for (size_t i = 0; i < index; i++) { it++; }
							frontier.push(*it);
						}
						break;
					}
					default:
						throw std::runtime_error("You idiot");
						break;
					}
					
					// 7.) --- Show results ---
					//showResults(movePos, matches, misses, faults);

					//copy(misses.begin(), misses.end(), inserter(allMisses, allMisses.end()));
					for (const auto& elem : misses) {
						allMisses.push_back(std::make_pair(movePos.position, elem.move));
					}
					//copy(faults.begin(), faults.end(), inserter(allFaults, allMisses.end()));
					for (const auto& elem : faults) {
						allMisses.push_back(std::make_pair(movePos.position, elem.move));
					}

					#ifdef _WIN32	 // TODO: fix this
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
					#endif

					//if (misses.size() || faults.size()) {
					//	cout << "Press any key...";
					//	cin.get();
					//}
				} // while(

				cout << "Done. All Results" << endl;
				showResults(allMisses, allFaults);

				cout << "...\n";
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Press any key...";
				cin.get();
				cout << "Press any key...";
				cin.get();
			} // stockfishAndForge()
		} // namespace movegen
	} // namespace test
} // namespace forge