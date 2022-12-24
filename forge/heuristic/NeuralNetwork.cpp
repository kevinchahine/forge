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
			//m_net.load(net_file);
			//
			//m_net.to(torch::kCPU);
		}
		
		heuristic_t NeuralNetwork::eval(const Position& pos)
		{
			cout << __FILE__ << " line " << __LINE__ << ". Don't use this method. Use other overload instead." << endl;
			return 999;
		}
		
		heuristic_t NeuralNetwork::eval(const Position& pos, bool whiteIsSearching)
		{			
			//// --- Input ---
			//// Tensor which holds input features (one-hot and multi-hot encodings)
			//torch::Tensor inputs = torch::ones({ 1, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE }, torch::kCPU);
			//
			//// --- Extract ---
			//// Extract one-hot encodings into a tensor
			//FeatureExtractor extractor;
			//extractor.init(pos, whiteIsSearching);
			//extractor.extractMaterial(inputs);
			//
			//// --- Evaluate Position ---
			//
			//torch::Tensor output = m_net.forward(inputs);
			//
			//// --- Return Evaluation ---
			//heuristic_t eval = output[0].item<float>();
			//
			//return eval;
			return 111;
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