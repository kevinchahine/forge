#pragma once

#include <iostream>

#include <torch/csrc/api/include/torch/data/transforms/tensor.h>

namespace forge
{
	class TensorPair
	{
	public:
		TensorPair() = default;
		TensorPair(const TensorPair&) = default;
		TensorPair(TensorPair&&) noexcept = default;
		~TensorPair() noexcept = default;
		TensorPair& operator=(const TensorPair&) = default;
		TensorPair& operator=(TensorPair&&) noexcept = default;

		TensorPair(int64_t nSamples, int64_t nInputFeatures, int64_t nOutputFeatures);

		int nSamples() const { return inputs.size(0); }

		friend std::ostream& operator<<(std::ostream& os, const TensorPair& pair);

	public:
		torch::Tensor inputs;
		torch::Tensor outputs;
	};
} // namespace forge