#include "CSVDataSet.h"

#include "CSVParser.h"
#include "TensorPair.h"
#include "forge/heuristic/FeatureExtractor.h"

#include <vector>

using namespace std;

namespace forge
{
	namespace ml
	{
		pair<torch::Tensor, torch::Tensor> readData(const string & pathToDataset, size_t batchSize)
		{
			// --- Parse CSV File ---
			CSVParser parser;
			parser.open(pathToDataset);
			vector<PositionEvalPair> batch = parser.getNextBatch();

			// --- Preprocess ---
			int64_t inputSize = forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE;

			// Create Tensors on CPU to speed up preprocessing
			TensorPair data{ 
				(int64_t) batch.size(),	// # of samples
				inputSize,				// # of input features
				1,						// # of output features
				torch::kCPU				// device
			};

			// Extract Features of each sample
			for (size_t sampleIndex = 0; sampleIndex < batch.size(); sampleIndex++) {
				const PositionEvalPair & pair = batch[sampleIndex];

				forge::heuristic::FeatureExtractor extractor;
				extractor.init(pair.position, true);	// true means that the evaluation is always in the perspective of the white player

				// --- Inputs ---
				torch::Tensor sampleSlice = data.inputs.slice(0, sampleIndex, sampleIndex + 1);

				extractor.extractMaterial(sampleSlice);

				// --- Output ---
				data.outputs[sampleIndex] = pair.eval;
			}

			return { std::move(data.inputs), std::move(data.outputs) };
		}

		CSVDataSet::CSVDataSet(const std::string & pathToDataset, size_t batchSize)
		{
			auto data = readData(pathToDataset, batchSize);

			_positions = std::move(data.first);
			_evaluations = std::move(data.second);
		}
	} // namespace ml
} // namespace forge