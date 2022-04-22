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

	void DataSet::generateNextBatch()
	{
		////if (m_parser.isOpen() == false) {
		////	throw std::runtime_error("dataset file is not open");
		////}
		////
		////// Load and Parse samples
		////vector<PositionEvalPair> pairs = m_parser.getNextBatch();
		////
		////// Extract Features
		////Eigen::Tensor<float, 2> features(pairs.size(), forge::FeatureExtractor::MATERIAL_FEATURES_SIZE + 1);	// + 1 includes the output attribute
		////
		////for (size_t i = 0; i < pairs.size(); i++) {
		////	const PositionEvalPair & pair = pairs[i];
		////
		////	const Position & pos = pair.position;
		////	heuristic_t eval = pair.eval;
		////
		////	forge::FeatureExtractor extractor;
		////	extractor.init(pos);
		////
		////	// --- Inputs ---
		////	auto f = extractor.extractMaterial();
		////	//Eigen::Tensor<float, 2> f = extractor.extractMaterial();
		////
		////	// copy into big tensor
		////	for (size_t col = 0; col < f.dimension(1); col++) {
		////		features(i, col) = f(0, col);
		////	}
		////
		////	// --- Output ---
		////	features(i, forge::FeatureExtractor::MATERIAL_FEATURES_SIZE) = eval;	// Last column
		////}
		////
		////// Add to Dataset
		////this->set(features);
	}

	void DataSet::print(std::ostream & os) const
	{
		////const Eigen::Tensor<float, 2> & data = this->get_data();
		////
		////for (size_t row = 0; row < data.dimension(0); row++) {
		////	os << '|';
		////
		////	for (size_t col = 0; col < data.dimension(1); col++) {
		////		os << setw(5) << data(row, col) << ' ';
		////	}
		////
		////	os << '|' << endl;
		////}
		////os << endl;
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
