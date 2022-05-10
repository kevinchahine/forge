#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>

#ifdef _WIN32
#include <xlnt/xlnt.hpp>	// TODO: Linux make this work on linux or find a replacement
#endif

namespace forge
{
	// Takes performance data and writes it to a spreadsheet file (.xlsx) 
	// Data is re-written to file as it is being collected.
	// Only intended to be used by AI Solvers.
	class PerformanceLogger
	{
	public:
		//void setOutputDir(const std::string & dirName);
		void setOutputDir(const std::filesystem::path & dirName);

		// Call this method once at the begining of a game.
		// This method, creates a new file called performance_log.xlsx if it
		// doesn't exist. Creates a new worksheet and fills in data about the 
		// solvers running the game.
		void start(
			const std::string & solverName,
			const std::string & solverVariant,
			const std::string & evaluationFunction,
			const std::string & moveGeneratorVersion);

		/*
		Writes data to spreadsheet file
		*/
		void log(
			int nodeCount, 
			float nodesPerSecond, 
			int plyCount, 
			float pliesPerSecond, 
			std::chrono::nanoseconds searchDuration);
	
		void log(
			int nodeCount,
			float nodesPerSecond,
			int plyCount,
			float pliesPerSecond,
			int uniqueNodesCount,
			std::chrono::nanoseconds searchDuration);

	public:
		std::string solverName = "";
		std::string solverVariant = "";
		std::string evaluationFunction = "";
		std::string moveGeneratorVersion = "";

	protected:
		xlnt::workbook m_workbook;
		std::filesystem::path m_output;
		const static std::string fileName;
	};
} // namespace forge