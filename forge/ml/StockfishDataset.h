#include <iostream>
#include <string>
#include <queue>
#include <filesystem>

#include "forge/heuristic/FeatureExtractor.h"

#include <torch/torch.h>

namespace forge
{
	namespace ml
	{
		class StockfishDataset : public torch::data::datasets::Dataset<StockfishDataset>
		{
		public: // -------------------- NESTED CLASSES ------------------------
			class FenEval
			{
				// !!! Make sure this remains an aggregate class !!!
				// !!! Or define a full constructor !!!
			public:
				static FenEval parse(const std::string& line);

				bool isValid() const { return fen.size(); }
				bool isInValid() const { return !isValid(); }

				friend std::ostream& operator<<(std::ostream& os, const FenEval& fe) {
					os << fe.fen << '\t' << fe.eval;

					return os;
				}

			public:
				std::string fen;
				int eval;
			};

			class TensorPair
			{
			public:
				size_t nSamples() const { return std::min(input.sizes()[0], output.sizes()[0]); }

				void resize(int64_t nSamples, const torch::Device& device) {

					// --- Input Tensor ---
					c10::IntArrayRef sizes = input.sizes();
					
					if (sizes.size() != 4 || 
						sizes.at(0) != nSamples ||
						sizes.at(1) != 8 ||
						sizes.at(2) != 8 ||
						sizes.at(3) != heuristic::FeatureExtractor::N_LAYERS) {

						input = torch::zeros({ nSamples, 8, 8, heuristic::FeatureExtractor::N_LAYERS }, device);
					}

					// --- Output Tensor ---

					if (output.sizes()[0] != nSamples || output.sizes()[1] != 1) {
						output = torch::zeros({ nSamples, 1 }, device);
					}

					input = input.to(device);
					output = output.to(device);
				}

				void resize(int64_t nSamples) {
					resize(nSamples, input.device());
				}

				TensorPair to(torch::Device& device) {
					TensorPair ret;

					ret.input = input.to(device);
					ret.output = output.to(device);

					return ret;
				}

				friend std::ostream& operator<<(std::ostream& os, const TensorPair& pair) {
					os << pair.input.device() << '\t'
						<< pair.output.device() << '\t';
					return os;
				}
			public:
				torch::Tensor input;
				torch::Tensor output;
			};

		public: // -------------------- METHODS -------------------------------

			StockfishDataset() = default;
			StockfishDataset(const StockfishDataset&);
			StockfishDataset(StockfishDataset&&) noexcept = default;
			~StockfishDataset() noexcept = default;
			StockfishDataset& operator=(const StockfishDataset&);
			StockfishDataset& operator=(StockfishDataset&&) noexcept = default;

			void open(const std::filesystem::path& csvPath);

			void skip(size_t nLines);

			const std::filesystem::path& filePath() const { return _csvFile; }

			void batchSize(size_t batchSize) { _batchSize = batchSize; }
			size_t batchSize() { return _batchSize; }

			torch::data::Example<> get(size_t index);

			torch::optional<size_t> size() const { return _nSamples; }

			void print();

			void saveProgress() const;

		private: // ------------------- METHODS -------------------------------
			void countSamples();

			void load();

		protected: // ----------------- FIELDS --------------------------------
			std::filesystem::path _csvFile;
			std::ifstream _in;// stream comming from 

			int64_t _batchSize = 10'000; // number of samples to load from the dataset
			int64_t _lineCount = 0;// which line are we at in the dataset
			int64_t _nSamples = 0;// total number of samples in dataset

			// TODO: rename:
			//	- preprocessingBatch, inferenceBatch
			//	- preBatch, postBatch
			TensorPair _cpuBatch;// Batch storing data on CPU. Used for preprocessing
			TensorPair _gpuBatch;// Batch storing data on GPU. Used for training/testing
			int64_t _gpuBatchIt = 0;
		};
	} // namespace ml
} // namespace forge