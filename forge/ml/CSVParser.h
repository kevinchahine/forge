#pragma once

#include "forge/ml/PositionEvalPair.h"

#include <iostream>
#include <fstream>

#include "forge/third_party/rapidcsv.h"

namespace forge
{
	class CSVParser
	{
	public:
		void open(const std::string & csvFilename);
		void close();
		bool isOpen() const;

		// Closes and reopens csv file so that it starts reading from the beginning
		void reset();

		void batchSize(size_t size) { m_batchSize = (size > 0 ? size : 1); /* batch must be atleast 1 row */ }
		size_t batchSize() const { return m_batchSize; }

		std::vector<PositionEvalPair> getNextBatch();
	
	protected:
		std::ifstream m_inFile;

		// Number of lines to read in each batch
		size_t m_batchSize = 100;

		rapidcsv::Document m_csvDoc;

		std::string csvFilename;
	};
} // namespace forge
