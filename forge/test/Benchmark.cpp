#include "Benchmark.h"

#include "forge/controllers/MCTS_Solver.h"
#include "forge/controllers/MCTS_Solver_MT.h"

#include "forge/heuristic/ApplePie.h"

#include "forge/test/positions.h"

#include <chrono>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/thread/thread.hpp>

#include <xlnt/xlnt.hpp>

using namespace std;

namespace forge
{
	namespace test
	{
		namespace benchmark
		{
			void mcts()
			{
				forge::test::init();	// initialize positions

				xlnt::workbook wb;
				//wb.load("benchmark.xlsx");
				
				xlnt::worksheet ws = wb.active_sheet();
				xlnt::row_t sequentialRow = xlnt::row_t(6);
				xlnt::column_t dataCol(2);
				xlnt::column_t avgNodesCol(g_positions.size() + 2);
				xlnt::column_t avgNPSCol(avgNodesCol + 1);

				MCTS_Solver sequentialMCTS;
				MCTS_Solver_MT concurrentMCTS;
				ws.cell("A1").value("Search");
				ws.cell("B1").value(sequentialMCTS.getName());

				sequentialMCTS.makeHeuristic<heuristic::ApplePie>();
				concurrentMCTS.makeHeuristic<heuristic::ApplePie>();
				ws.cell("A2").value("Heuristic");
				ws.cell("B2").value(sequentialMCTS.heuristicPtr()->name());

				chrono::seconds timeLimit = chrono::seconds(4);
				sequentialMCTS.searchMonitor().timeLimit = timeLimit;
				concurrentMCTS.searchMonitor().timeLimit = timeLimit;
				ws.cell("A3").value("Search Time (seconds)");
				ws.cell("B3").value(timeLimit.count());

				ws.cell("A4").value("# of Nodes Explored");
				ws.cell("A5").value("Thread Count");
				ws.merge_cells(xlnt::range_reference(xlnt::column_t(2), xlnt::row_t(4), xlnt::column_t(g_positions.size() + 1), xlnt::row_t(4)));
				ws.cell("B4").value("Positions");

				// --- Headers --- 
				ws.cell(xlnt::column_t(g_positions.size() + 2), xlnt::row_t(5)).value("nodes");
				ws.cell(xlnt::column_t(g_positions.size() + 3), xlnt::row_t(5)).value("nps");

				const size_t CONCURRENCY = boost::thread::hardware_concurrency();

				// --- Left Headers ---
				ws.cell(xlnt::column_t(1), sequentialRow).value("sequential");

				for (size_t t = 1; t <= CONCURRENCY; t++) {
					xlnt::cell c = ws.cell(xlnt::column_t(1), xlnt::row_t(sequentialRow + t));
					c.value(t);
				}

				// --- Averages ---
				for (size_t t = 0; t <= CONCURRENCY; t++) {
					{
						stringstream formula;
						formula << "average("
							<< dataCol.column_string() << sequentialRow + t << ":"
							<< (avgNodesCol - 1).column_string() << sequentialRow + t
							<< ")";

						ws.cell(avgNodesCol, sequentialRow + t).formula(formula.str());
					}

					{
						stringstream formula;

						formula << avgNodesCol.column_string() << sequentialRow + t << "/" << timeLimit.count();

						ws.cell(avgNPSCol, sequentialRow + t).formula(formula.str());
					}
				}

				// --- Positions and Data ---
				for (size_t i = 0; i < g_positions.size(); i++) {
					// --- Position Header ---
					const Position& position = g_positions.at(i);
					const std::string fen = position.toFEN();
					cout << "----- Position " << i << ' ' << fen << "-----" << endl;

					xlnt::column_t positionCol = xlnt::column_t(i + 2);
					
					ws.cell(positionCol, xlnt::row_t(5)).value(fen);
					
					position.board().printMini();
					
					// --- Sequential ---
					//cout << sequentialMCTS.getName() << ' ' << sequentialMCTS.getNameVariant() << endl;
					//try {
					//	sequentialMCTS.getMove(position);
					//	sequentialMCTS.searchMonitor().print();
					//	ws.cell(positionCol, sequentialRow).value(sequentialMCTS.searchMonitor().nodeCount.value());
					//}
					//catch (std::exception& e) {
					//	cout << "Exception: " << e.what() << endl
					//		<< "\tSkipping this sample." << endl;
					//	ws.cell(xlnt::column_t(i + 2), sequentialRow).value("exception occured");
					//}
					
					wb.save("benchmark.xlsx");
					
					// --- Concurrent ---
					cout << concurrentMCTS.getName() << ' ' << concurrentMCTS.getNameVariant() << endl;
					
					for (size_t t = 1; t <= CONCURRENCY; t++) {
						cout << "Thread Count = " << t << endl;
						concurrentMCTS.threadCount() = t;
					
						xlnt::row_t row = xlnt::row_t(sequentialRow + t);
					
						try
						{
							concurrentMCTS.getMove(position);
								
							concurrentMCTS.searchMonitor().print();
							
							ws.cell(positionCol, row).value(concurrentMCTS.searchMonitor().nodeCount.value());
						}
						catch (const std::exception& e)
						{
							cout << "Exception: " << e.what() << endl
								<< "\tSkipping this sample." << endl;
							ws.cell(positionCol, row).value("exception occured");
						}
					
						wb.save("benchmark.xlsx");
					}

					cout << endl;
				}

				wb.save("benchmark.xlsx");
			}
			
			void mcts_phases()
			{
				string filename = "benchmark_mcts_phases.xlsx";
				//boost::filesystem::path path = filename;

				forge::test::init();	// initialize positions

				xlnt::workbook wb;

				//if (boost::filesystem::exists(path)) {
				//	cout << "Loading existing workbook \'" << filename << "\'...";
				//	wb.load(filename);
				//	cout << "done" << endl;
				//}
				
				xlnt::worksheet ws = wb.create_sheet();

				xlnt::row_t headerRow = xlnt::row_t(6);

				int col = 0;
				xlnt::column_t positionCol(++col);
				xlnt::column_t selectionCol(++col);
				xlnt::column_t evaluationCol(++col);
				xlnt::column_t expansionCol(++col);
				xlnt::column_t backpropCol(++col);
				xlnt::column_t totalCol(++col);

				MCTS_Solver mcts;

				ws.cell("A1").value("Search:");
				ws.cell("B1").value(mcts.getName());

				mcts.makeHeuristic<heuristic::ApplePie>();
				ws.cell("A2").value("Heuristic:");
				ws.cell("B2").value(mcts.heuristicPtr()->name());

				chrono::seconds timeLimit = chrono::seconds(4);
				mcts.searchMonitor().timeLimit = timeLimit;
				ws.cell("A3").value("Search Time (seconds)");
				ws.cell("B3").value(timeLimit.count());

				// --- Headers --- 
				ws.cell(selectionCol, headerRow - 1).value("Execution time (msec)");
				ws.merge_cells(xlnt::range_reference(selectionCol, headerRow - 1, totalCol, headerRow - 1));
				
				ws.cell(positionCol, headerRow).value("position");
				ws.cell(selectionCol, headerRow).value("selection");
				ws.cell(evaluationCol, headerRow).value("evaluation");
				ws.cell(expansionCol, headerRow).value("expansion");
				ws.cell(backpropCol, headerRow).value("backpropagation");
				ws.cell(totalCol, headerRow).value("total");

				for (size_t p = 0; p < g_positions.size(); p++) {
					try
					{
						const Position& pos = g_positions.at(p);
						const string& fen = pos.toFEN();

						cout << "----- Position " << p << ' ' << fen << "-----" << endl;
						pos.board().printMini();
						xlnt::row_t row(headerRow + p + 1);

						ws.cell(positionCol, row).value(fen);

						mcts.getMove(pos);

						const auto& sm = mcts.searchMonitor();
						ws.cell(selectionCol, row).value(chrono::duration_cast<chrono::milliseconds>(sm.selection.elapsed()).count());
						ws.cell(evaluationCol, row).value(chrono::duration_cast<chrono::milliseconds>(sm.evaluation.elapsed()).count());
						ws.cell(expansionCol, row).value(chrono::duration_cast<chrono::milliseconds>(sm.expansion.elapsed()).count());
						ws.cell(backpropCol, row).value(chrono::duration_cast<chrono::milliseconds>(sm.backprop.elapsed()).count());
						ws.cell(totalCol, row).value(chrono::duration_cast<chrono::milliseconds>(sm.searchTime.elapsed()).count());

						cout << "Saving to " << filename << "...";
						wb.save(filename);
						cout << "done" << endl;
					}
					catch (const std::exception& e)
					{
						cout << e.what() << endl;
					}
				}

				wb.save(filename);
			}
		} // namespace benchmark
	} // namespace test
} // namespace forge