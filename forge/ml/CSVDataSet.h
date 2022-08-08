#pragma once

#include <iostream>
#include <string>

#include <torch/torch.h>	// TODO: break this down into only what we need

namespace forge
{
	namespace ml
	{
		class CSVDataSet : public torch::data::datasets::Dataset<CSVDataSet>
		{
		public:
			explicit CSVDataSet(const std::string & pathToDataset, size_t batchSize = 1'000);

			// Returns the `Example` at the given `index`
			torch::data::Example<> get(size_t index) override { return { _positions[index], _evaluations[index] }; }

			// Returns number of samples in the subset
			torch::optional<size_t> size() const override { return _positions.size(0); }

			// Returns all positions in subset (inputs)
			const torch::Tensor & positions() const { return _positions; }

			// Returns all evaluations in subset (targets)
			const torch::Tensor & evaluations() const { return _evaluations; }
			
		private:

			torch::Tensor _positions;
			torch::Tensor _evaluations;
		};
	} // namespace ml
} // namespace forge