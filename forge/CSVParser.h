#pragma once

#include "PositionEvalPair.h"

#include <iostream>
#include <fstream>

#include "rapidcsv.h"

namespace forge
{
	class CSVParser
	{
	public:
		void open(const std::string & csvFilename);
		void close();
		bool isOpen() const;

		void batchSize(size_t size) { m_batchSize = (size > 0 ? size : 1); /* batch must be atleast 1 row */ }
		size_t batchSize() const { return m_batchSize; }

		vector<PositionEvalPair> getNextBatch();

	protected:
		std::ifstream m_inFile;

		// Number of lines to read in each batch
		size_t m_batchSize = 100;

		rapidcsv::Document m_csvDoc;
	};
} // namespace forge
