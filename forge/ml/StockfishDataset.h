#include <iostream>
#include <string>
#include <queue>
#include <filesystem>

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

				void to(torch::Device& device) {
					input = input.to(device);
					output = output.to(device);
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

			TensorPair _batch;
			int64_t _batchIt = 0;
		};
	} // namespace ml
} // namespace forge