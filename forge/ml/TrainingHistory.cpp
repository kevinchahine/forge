#include "forge/ml/TrainingHistory.h"

#include "forge/third_party/rapidcsv.h"

using namespace std;

namespace forge
{
	namespace ml
	{
		void TrainingHistory::from(const std::string & filename)
		{
			try {
				ifstream inFile;

				inFile.open(filename);
				
				this->from(inFile);
				
				inFile.close();
			}
			catch (exception & e) {
				cout << "Exception caught: " << e.what() << endl;
			}
		}

		void TrainingHistory::from(std::ifstream & inFile)
		{
			//m_csvDoc = rapidcsv::Document(inFile, rapidcsv::LabelParams(-1, -1));	// Treat 1st line as a sample (not a header)
			//
			//for (size_t rowIndex = 0; rowIndex < m_csvDoc.GetRowCount(); rowIndex++) {
			//	size_t epoch = m_csvDoc.GetCell<size_t>("epoch", rowIndex);
			//	float train = m_csvDoc.GetCell<float>("train", rowIndex);
			//	float test = m_csvDoc.GetCell<float>("test", rowIndex);
			//
			//	this->deque<LossPoint>::emplace_back(epoch, train, test);
			//}
		}

		void TrainingHistory::setOutFile(const std::string & outFile)
		{
			
		}

		void TrainingHistory::setOutFile(std::ofstream && outFile){}

		void TrainingHistory::push_back(const LossPoint & loss){}

		void TrainingHistory::emplace_back(LossPoint && loss){}

		void TrainingHistory::flush(){}

		void TrainingHistory::plot(){}

		void TrainingHistory::getPlot(){}
	} // namespace ml
} // namespace forge