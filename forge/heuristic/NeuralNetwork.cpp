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

		heuristic_t NeuralNetwork::eval(const Position& pos, bool whiteIsSearching)
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
			
			//m_net.to(forge::g_computingDevice);
			//inputs = inputs.to(forge::g_computingDevice);
			torch::Tensor output = m_net.forward(inputs);

			// --- Return Evaluation ---
			heuristic_t eval = output[0].item<float>();

			return (whiteIsSearching ? eval : -eval);
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