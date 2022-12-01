#include "NeuralNetwork.h"

#include "forge/heuristic/FeatureExtractor.h"
#include "forge/source/globals.h"

#include <chrono>

using namespace std;

namespace forge
{
	namespace heuristic
	{
		NeuralNetwork::NeuralNetwork(const string& net_file)
		{
			m_net.load(net_file);

			m_net.to(torch::kCPU);
		}

		heuristic_t NeuralNetwork::eval(const Position& pos)
		{
			// --- Input ---
			// Tensor which holds input features (one-hot and multi-hot encodings)
			torch::Tensor inputs = torch::ones({ 1, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE }, torch::kCPU);

			// --- Extract ---
			// Extract one-hot encodings into a tensor
			FeatureExtractor extractor;
			extractor.init(pos);
			extractor.extractMaterial(inputs);

			// --- Evaluate Position ---
			//static chrono::high_resolution_clock::duration forwardTime;	// op
			//static size_t nForwards = 0;

			//chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();	// op
			//m_net.to(forge::g_computingDevice);
			//inputs = inputs.to(forge::g_computingDevice);
			torch::Tensor output = m_net.forward(inputs);
			//chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();	// op

			//forwardTime += (endTime - startTime);																				// op
			//++nForwards;																												// op
			//
			//static chrono::system_clock::time_point lastPrint = chrono::system_clock::now();
			//chrono::system_clock::duration sinceLastPrint = chrono::system_clock::now() - lastPrint;
			//
			//if (chrono::duration_cast<chrono::milliseconds>(sinceLastPrint).count() > 1'000) {	
			//	lastPrint = chrono::system_clock::now();																		// op
			//
			//	cout << "==== " 																								// op
			//		<< nForwards << " forwards in " 																			// op
			//		<< chrono::duration_cast<chrono::seconds>(forwardTime).count() << " sec. "									// op
			//		<< double(nForwards) / double(chrono::duration_cast<chrono::milliseconds>(forwardTime).count()) * 1000.0 << " ff/sec"		// op
			//		<< endl;																									// op
			//}																													// op

			// --- Return Evaluation ---
			heuristic_t eval = output[0].item<float>();
			return eval;
		}

		unique_ptr<Base> NeuralNetwork::clone() const
		{
			return make_unique<NeuralNetwork>(*this);
		}

		void NeuralNetwork::print(const Position& pos, std::ostream& os) const
		{
			cout << __FUNCTION__ << " not fully implemented" << endl;
		}
	} // namespace heuristic
} // namespace forge