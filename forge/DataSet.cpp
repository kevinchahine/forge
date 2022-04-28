#include "DataSet.h"

#include <fstream>

using namespace std;

namespace forge
{
	void DataSet::openFile(const std::string & filename)
	{
		cout << "--- CSVParser ---" << endl;
		cout << "Opening file: " << filename << endl;

		m_parser.open(filename);
	}

	void DataSet::closeFile()
	{
		m_parser.close();
	}

	void DataSet::reset()
	{
		m_parser.reset();
	}

	TensorPair DataSet::getNextBatch()
	{
		if (m_parser.isOpen() == false) {
			throw std::runtime_error("dataset file is not open");
		}
		
		// Load and Parse samples
		vector<PositionEvalPair> pairs = m_parser.getNextBatch();
		
		TensorPair data{ (int64_t)pairs.size(), forge::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1 };

		// Extract Features of each sample
		for (size_t sampleIndex = 0; sampleIndex < pairs.size(); sampleIndex++) {
			const PositionEvalPair & pair = pairs[sampleIndex];
			
			forge::FeatureExtractor extractor;
			extractor.init(pair.position);
		
			// --- Inputs ---
			data.inputs[sampleIndex] = extractor.extractMaterial();
			
			// --- Output ---
			data.outputs[sampleIndex] = pair.eval;
		}

		return data;
	}

	void DataSet::toCSV(std::ostream & os) const
	{
		////const Eigen::Tensor<float, 2> & data = this->get_data();
		////
		////for (size_t row = 0; row < data.dimension(0); row++) {
		////	for (size_t col = 0; col < data.dimension(1); col++) {
		////		os << data(row, col) << ", ";
		////	}
		////	os << endl;
		////}
		////os << endl;
	}

	void DataSet::toCSV(const std::string & filename) const
	{
		////ofstream out;
		////
		////try {
		////	out.open(filename);
		////
		////	this->toCSV(out);
		////
		////	out.close();
		////}
		////catch (exception & e) {
		////	cout << "Exception caught " << __FILE__ << " line " << __LINE__ << ": " << e.what() << endl;
		////};
	}

} // namespace forge
