#include "Benchmark.h"

#include "forge/controllers/MCTS_Solver.h"
#include "forge/controllers/MCTS_Solver_MT.h"

#include "forge/heuristic/Material.h"

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

				sequentialMCTS.makeHeuristic<heuristic::Material>();
				concurrentMCTS.makeHeuristic<heuristic::Material>();
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
				xlnt::column_t nodeCol(++col);

				MCTS_Solver mcts;

				ws.cell("A1").value("Search:");
				ws.cell("B1").value(mcts.getName());

				mcts.makeHeuristic<heuristic::Material>();
				ws.cell("A2").value("Heuristic:");
				ws.cell("B2").value(mcts.heuristicPtr()->name());

				chrono::seconds timeLimit = chrono::seconds(4);
				mcts.searchMonitor().timeLimit = timeLimit;
				ws.cell("A3").value("Search Time (seconds)");
				ws.cell("B3").value(timeLimit.count());

				// --- Headers --- 
				ws.cell(selectionCol, headerRow - 1).value("Execution time (nsec / node)");
				ws.merge_cells(xlnt::range_reference(selectionCol, headerRow - 1, totalCol, headerRow - 1));
				
				ws.cell(positionCol, headerRow).value("position");
				ws.cell(selectionCol, headerRow).value("selection");
				ws.cell(evaluationCol, headerRow).value("evaluation");
				ws.cell(expansionCol, headerRow).value("expansion");
				ws.cell(backpropCol, headerRow).value("backpropagation");
				ws.cell(totalCol, headerRow).value("total");
				ws.cell(nodeCol, headerRow).value("node count");

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
						size_t nc = sm.nodeCount.value();
						int expansionCount = sm.expansionCount.value();
						const auto sl = /*chrono::duration_cast<chrono::milliseconds>*/(sm.selection.elapsed() / nc).count();
						const auto ev = /*chrono::duration_cast<chrono::milliseconds>*/(sm.evaluation.elapsed() / nc).count();
						const auto ex = /*chrono::duration_cast<chrono::milliseconds>*/(sm.expansion.elapsed() / expansionCount).count();
						const auto bp = /*chrono::duration_cast<chrono::milliseconds>*/(sm.backprop.elapsed() / nc).count();
						const auto tl = /*chrono::duration_cast<chrono::milliseconds>*/(sm.searchTime.elapsed() / nc).count();

						cout
							<< "selection:       " << sl << " nsec / node" << endl
							<< "evaluation:      " << ev << " nsec / node" << endl
							<< "expansion:       " << ex << " nsec / node" << endl
							<< "backpropagation: " << bp << " nsec / node" << endl
							<< "total:           " << tl << " nsec / node" << endl;

						ws.cell(selectionCol, row).value(sl);
						ws.cell(evaluationCol, row).value(ev);
						ws.cell(expansionCol, row).value(ex);
						ws.cell(backpropCol, row).value(bp);
						ws.cell(totalCol, row).value(tl);
						ws.cell(nodeCol, row).value(nc);

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

			void mcts_phases_without_search()
			{
				string filename = "benchmark_mcts_phases_without_search.xlsx";
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
				xlnt::column_t emptyCol(++col);

				MCTS_Solver mcts;

				ws.cell("A1").value("Search:");
				ws.cell("B1").value(mcts.getName());

				mcts.makeHeuristic<heuristic::Material>();
				ws.cell("A2").value("Heuristic:");
				ws.cell("B2").value(mcts.heuristicPtr()->name());

				chrono::seconds timeLimit = chrono::seconds(4);
				mcts.searchMonitor().timeLimit = timeLimit;
				ws.cell("A3").value("Search Time (seconds)");
				ws.cell("B3").value(timeLimit.count());

				// --- Headers --- 
				ws.cell(selectionCol, headerRow - 1).value("Execution time (nsec)");
				ws.merge_cells(xlnt::range_reference(selectionCol, headerRow - 1, emptyCol, headerRow - 1));

				ws.cell(positionCol, headerRow).value("position");
				ws.cell(selectionCol, headerRow).value("selection");
				ws.cell(evaluationCol, headerRow).value("evaluation");
				ws.cell(expansionCol, headerRow).value("expansion");
				ws.cell(backpropCol, headerRow).value("backpropagation");

				unique_ptr<forge::heuristic::Base> heuristic = make_unique<forge::heuristic::Material>();

				for (size_t p = 0; p < g_positions.size(); p++) {
					try
					{
						const Position& pos = g_positions.at(p);
						const string& fen = pos.toFEN();

						//cout << "----- Position " << p << ' ' << fen << "-----" << endl;
						//pos.board().printMini();
						xlnt::row_t row(headerRow + p + 1);

						ws.cell(positionCol, row).value(fen);

						//mcts.getMove(pos);
						MCTS_Node node;
						node.position() = pos;
						MCTS_Node::iterator it = node.root();

						StopWatch selection;
						StopWatch evaluation;
						StopWatch expansion;
						StopWatch backpropagation;
						StopWatch empty;

						// --- evaluation ---
						evaluation.resume();
						bool maximizeWhite = (*it).position().moveCounter().isBlacksTurn();
						heuristic_t eval = heuristic->eval((*it).position(), maximizeWhite);
						evaluation.pause();

						// --- expansion ---
						expansion.resume();
						(*it).expand();
						expansion.pause();

						// --- selection ---
						selection.resume();
						if ((*it).isPruned() == false && it.hasChildren()) {
							it.toBestUCB();
						}
						selection.pause();

						// --- backpropagation ---
						backpropagation.resume();
						while (it.isRoot() == false) {
							(*it).update(eval);
							eval = -eval;
							it.toParent();
						}

						// --- empty ---
						empty.resume();
						empty.pause();

						it = node.root();
						backpropagation.pause();

						const auto s =  selection.elapsed().count();
						const auto ev = evaluation.elapsed().count();
						const auto ex = expansion.elapsed().count();
						const auto bp = backpropagation.elapsed().count();
						const auto ep = empty.elapsed().count();

						//cout
						//	<< "selection:       " << sl << endl
						//	<< "evaluation:      " << ev << endl
						//	<< "expansion:       " << ex << endl
						//	<< "backpropagation: " << bp << endl
						//	;

						ws.cell(selectionCol, row).value(s);
						ws.cell(evaluationCol, row).value(ev);
						ws.cell(expansionCol, row).value(ev);
						ws.cell(backpropCol, row).value(bp);
						ws.cell(emptyCol, row).value(ep);

						//cout << "Saving to " << filename << "...";
						//wb.save(filename);
						//cout << "done" << endl;
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