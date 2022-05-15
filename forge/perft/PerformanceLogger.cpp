#include "PerformanceLogger.h"

#include <Guten/termcolor/termcolor.hpp>

#include <algorithm>
#include <filesystem>

#include <boost/date_time.hpp>	// TODO: Minimize this into only what we need

using namespace std;

namespace forge
{
	const string PerformanceLogger::fileName = "performance_log.xlsx";

	//void PerformanceLogger::setOutputDir(const string & dirName)
	//{
	//	setOutputDir(filesystem::path(dirName));
	//}
	
	void PerformanceLogger::setOutputDir(const filesystem::path & dirName)
	{
		// --- Start by setting output directory ---
		m_output = dirName;
		filesystem::path dir = m_output;
	
		// --- Add filename to directory path ---
		m_output.append("performance_log.xlsx");
		cout << "m_output = " << m_output << '\n';
	
		// --- See if this file already exists ---
		filesystem::directory_iterator it(dir);
		filesystem::directory_iterator end;
	
		// Compares if a path is equal to m_output and it is a regular file
		auto comp_files = [&](const filesystem::directory_entry & entry) {
			return entry.path() == m_output;
		};
	
		filesystem::directory_iterator file_it = find_if(it, end, comp_files);
	
		// Did we find the file?
		if (file_it == end) {
			// No. But we can create it.
			
		}
		else {
			// Yes. File was found. We need to read it into a workbook and add data to it
		}
	}

	void PerformanceLogger::start(
		const string & solverName, 
		const string & solverVariant, 
		const string & evaluationFunction, 
		const string & moveGeneratorVersion)
	{
		#ifdef _WIN32
		filesystem::path currDir = filesystem::current_path();
		
		filesystem::directory_iterator it(filesystem::current_path());
		filesystem::directory_iterator end;
		
		filesystem::path filePath = currDir / fileName;
		
		filesystem::directory_iterator fileIt = find(it, end, filePath);
		
		xlnt::workbook book;
		
		if (fileIt == end) {
			// File doesn't exist. Do nothing. Well create it later.
		}
		else {
			book.load(filePath);
		}
		
		xlnt::worksheet sheet = book.create_sheet();
		
		sheet.cell(1, 1).value("Solver Name");
		sheet.cell(1, 2).value(solverName);
		
		sheet.cell(2, 1).value("Solver Variant");
		sheet.cell(2, 2).value(solverVariant);
		
		sheet.cell(3, 1).value("Evaluation Function");
		sheet.cell(3, 2).value(evaluationFunction);
		
		sheet.cell(4, 1).value("Move Gen Version");
		sheet.cell(4, 2).value(moveGeneratorVersion);
		
		sheet.cell(5, 1).value("Date");
		sheet.cell(5, 2).value("???");	// How about some dates
		// --- Leave row 3 empty ---
		
		sheet.cell(1, 4).value("Node Count");
		sheet.cell(2, 4).value("Nodes Per Second");
		sheet.cell(3, 4).value("Ply Count");
		sheet.cell(4, 4).value("Plys Per Second");
		sheet.cell(5, 4).value("Search Duration");
		sheet.cell(6, 4).value("Unique Nodes Searched");

		book.save(filePath);
		#endif
	}

	void PerformanceLogger::log(
		int nodeCount, 
		float nodesPerSecond, 
		int plyCount, 
		float pliesPerSecond, 
		chrono::nanoseconds searchDuration)
	{
		filesystem::path currDir = filesystem::current_path();
		
		filesystem::directory_iterator it(currDir);
		filesystem::directory_iterator end;
		
		filesystem::path filePath = currDir / fileName;
		
		filesystem::directory_iterator fileIt = find(it, end, filePath);
		
		xlnt::workbook book;
		
		if (fileIt == end) {
			// File does not exist
		}
		else {
			book.load(filePath);
		}
		
		// Create a new sheet after the existing ones
		xlnt::worksheet sheet = book.sheet_by_index(book.sheet_count() - 1);
		
		// Insert performance metrics
		xlnt::row_t row = 4;
		while (true) {
			xlnt::cell cell = sheet.cell(1, row);
		
			if (cell.to_string().empty()) {
				break;
			}
		
			row++;
		}
		
		sheet.cell(1, row).value(nodeCount);
		sheet.cell(2, row).value(nodesPerSecond);
		sheet.cell(3, row).value(plyCount);
		sheet.cell(4, row).value(pliesPerSecond);
		sheet.cell(5, row).value(chrono::duration<float, ratio<1, 1>>(searchDuration).count());
		
		book.save(filePath);
	}

	void PerformanceLogger::log(int nodeCount, float nodesPerSecond, int plyCount, float pliesPerSecond, int uniqueNodesCount, std::chrono::nanoseconds searchDuration)
	{
		filesystem::path currDir = filesystem::current_path();

		filesystem::directory_iterator it(currDir);
		filesystem::directory_iterator end;

		filesystem::path filePath = currDir / fileName;

		filesystem::directory_iterator fileIt = find(it, end, filePath);

		xlnt::workbook book;

		if (fileIt == end) {
			// File does not exist
		}
		else {
			book.load(filePath);
		}

		// Create a new sheet after the existing ones
		xlnt::worksheet sheet = book.sheet_by_index(book.sheet_count() - 1);

		// Look for the 1st empty row
		xlnt::row_t row = 4;
		while (true) {
			xlnt::cell cell = sheet.cell(1, row);

			if (cell.to_string().empty()) {
				break;
			}

			row++;
		}

		// Insert performance metrics
		sheet.cell(1, row).value(nodeCount);
		sheet.cell(2, row).value(nodesPerSecond);
		sheet.cell(3, row).value(plyCount);
		sheet.cell(4, row).value(pliesPerSecond);
		sheet.cell(5, row).value(chrono::duration<float, ratio<1, 1>>(searchDuration).count());
		sheet.cell(6, row).value(uniqueNodesCount);

		book.save(filePath);
	}
} // namespace forge