#include "NeuralNetworkHeuristic.h"

#include "forge/heuristics/FeatureExtractor.h"

using namespace std;

namespace forge
{
	NeuralNetworkHeuristic::NeuralNetworkHeuristic(const string& net_file)
	{
		m_net.load(net_file);

		m_net.to(torch::kCPU);
	}

	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		// --- Input ---
		// Tensor which holds input features (one-hot and multi-hot encodings)
		torch::Tensor inputs = torch::ones({1, forge::FeatureExtractor::MATERIAL_FEATURES_SIZE}, torch::kCPU);

		// --- Extract ---
		// Extract one-hot encodings into a tensor
		FeatureExtractor extractor;
		extractor.init(pos);
		extractor.extractMaterial(inputs);

		// --- Evaluate Position ---
		torch::Tensor output = m_net.forward(inputs);

		// --- Return Evaluation ---
		heuristic_t eval = output[0].item<float>();
		return eval;
	}

	unique_ptr<HeuristicBase> NeuralNetworkHeuristic::clone() const
	{
		return make_unique<NeuralNetworkHeuristic>(*this);
	}

	void NeuralNetworkHeuristic::print(const Position& pos, std::ostream& os) const
	{
		cout << __FUNCTION__ << " not fully implemented" << endl;
	}

} // namespace forge