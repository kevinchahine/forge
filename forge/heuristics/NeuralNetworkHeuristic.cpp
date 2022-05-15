#include "NeuralNetworkHeuristic.h"

using namespace std;

namespace forge
{
	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		////size_t inputSize = m_model.get_layer_pointer(0)->get_inputs_number();

		////Eigen::Tensor<float, 2> inputs(1, inputSize);	// TODO: do this in 2D. Let 1st dim be the feature and the 2nd be the square
		////Eigen::Tensor<float, 2> outputs(1, 1);

		// --- Preprocess ---

		// --- Feature Extraction ---
		////inputs = this->featureExtraction(pos);

		// --- Feed Forward ---
		////outputs = m_model.calculate_outputs(inputs);

		// --- Return ---
		return 0.0;//// outputs(0, 0);
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