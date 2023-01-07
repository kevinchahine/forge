#include "NeuralNetwork.h"

#include "forge/heuristic/FeatureExtractor.h"
#include "forge/source/globals.h"

#include <chrono>

using namespace std;

namespace forge
{
	namespace heuristic
	{
		NeuralNetwork::NeuralNetwork(const string & net_file) {
			torch::load(m_net, net_file);

			//m_net.to(torch::kCPU);
		}

		heuristic_t NeuralNetwork::eval(const Position & pos) {
			cout << __FILE__ << " line " << __LINE__ << ". Don't use this method. Use other overload instead." << endl;
			return 999;
		}

		heuristic_t NeuralNetwork::eval(const Position & pos, bool whiteIsSearching) {
			// --- Input ---
			// Tensor which holds input features (one-hot and multi-hot encodings)
			// TODO: reuse the inputsCPU tensor to prevent slow tensor reallocation
			torch::Tensor inputs = torch::zeros({ 1, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE }, torch::kCPU);

			// --- Extract ---
			// Extract one-hot encodings into a tensor
			FeatureExtractor extractor;
			extractor.init(pos, whiteIsSearching);
			torch::Tensor sampleSlice = inputs.slice(0, 0, 1);// _cpuBatch.input.slice(0, count, count + 1);
			extractor.extractMaterial(sampleSlice);
			//sampleSlice = sampleSlice.to(torch::kCUDA);

			// --- Evaluate Position ---

			//cout << sampleSlice << endl;

			//m_net->to(torch::kCUDA);
			m_net->to(torch::kCPU);

			torch::Tensor output = m_net->forward(sampleSlice);

			// --- Return Evaluation ---
			heuristic_t eval = output[0].template item<float>();

			return eval;
		}

		vector<heuristic_t> NeuralNetwork::eval(const vector<const Position *> & positions, bool whiteIsSearching) {
			int nSamples = positions.size();

			inputsCPU = inputsCPU.resize_({ nSamples, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE });

			// --- Extract ---
			// Extract one-hot encodings into a tensor
			FeatureExtractor extractor;

			for (int i = 0; i < nSamples; i++) {
				//for (T it = begin, size_t sampleIndex = 0; it != end; it++, sampleIndex++) {
				const Position & position = *positions.at(i);

				extractor.init(position, whiteIsSearching);

				torch::Tensor sampleSlice = inputsCPU.slice(0, i, i + 1);

				extractor.extractMaterial(sampleSlice);
			}

			// --- Evaluate Position ---

			//m_net->to(torch::kCUDA);
			m_net->to(torch::kCPU);
			torch::Tensor inputsDevice = inputsCPU.to(torch::kCPU);

			torch::Tensor output = m_net->forward(inputsDevice);

			output = output.to(torch::kCPU);

			// --- Return Evaluation ---
			std::vector<heuristic_t> evals(nSamples);

			auto accessor = output.accessor<float, 2>();	// Good for CPU access

			for (size_t i = 0; i < nSamples; i++) {
				evals.at(i) = accessor[i][0];//.template item<float>();
				//heuristic_t eval = output[0].template item<float>();
			}

			return evals;
		}

		unique_ptr<Base> NeuralNetwork::clone() const {
			return make_unique<NeuralNetwork>(*this);
		}

		void NeuralNetwork::print(const Position & pos, std::ostream & os) const {
			cout << __FUNCTION__ << " not fully implemented" << endl;
		}
	} // namespace heuristic
} // namespace forge