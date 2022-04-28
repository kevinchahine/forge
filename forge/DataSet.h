#pragma once

#include "CSVParser.h"
#include "FeatureExtractor.h"

#include <iostream>
#include <string>

#include "ml/TensorPair.h"

namespace forge
{
	// TODO: Maybe derive this class into CSVDataSet
	class DataSet
	{
	public:
		// Opens a csv file
		void openFile(const std::string & filename);

		void closeFile();

		// Closes and reopens dataset so that it starts reading from the beginning
		// This method is good to call when training is not finished but the
		// last batch of data has been extracted or when the last batch comes out empty.
		void reset();

		void batchSize(size_t batchSize) { m_parser.batchSize(batchSize); }
		size_t batchSize() const { return m_parser.batchSize(); }

		// Loads next samples from dataset file
		// Parses and Extracts features into Tensors
		// And stores them in this object.
		// Training Tensors can be accessed by calling the method OpenNN::DataSet::data()
		// ex:
		//	DataSet ds;
		//	ds.data();		// Returns Eigen::Tensor<float, 2>
		TensorPair getNextBatch();

		void toCSV(std::ostream & os) const;

		void toCSV(const std::string & filename) const;

	protected:
		CSVParser m_parser;				// Reads batches of samples from dataset file and parses them into PositionEvalPairs

		FeatureExtractor m_extractor;	// Extracts features from Positions and stores them as Tensors
	};
} // namespace forge
