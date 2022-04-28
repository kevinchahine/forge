#pragma once

#include "../forge/DataSet.h"

#include <iostream>
#include <string>

#include "torch\csrc\api\include\torch\nn\module.h"
#include "torch\csrc\api\include\torch\nn\modules\linear.h"

namespace forge
{
	namespace ml
	{
		class Net : public torch::nn::Module
		{
		public:
			Net() {
				fc1 = register_module("fc1", torch::nn::Linear(832, 64));
				fc2 = register_module("fc2", torch::nn::Linear(64, 32));
				fc3 = register_module("fc3", torch::nn::Linear(32, 1));
			}
			Net(const Net&) = default;
			Net(Net&&) noexcept = default;
			~Net() noexcept = default;
			Net& operator=(const Net&) = default;
			Net& operator=(Net&&) noexcept = default;

			torch::Tensor forward(torch::Tensor x) {
				x = torch::relu(fc1->forward(x.reshape( { x.size(0), 832 } )));
				x = torch::dropout(x, 0.5, is_training());
				x = torch::relu(fc2->forward(x));
				x = torch::log_softmax(fc3->forward(x), 1);
				return x;
			}

			void train(DataSet& trainingDS, size_t nEpochs);

		public:
			torch::nn::Linear fc1{ nullptr };
			torch::nn::Linear fc2{ nullptr };
			torch::nn::Linear fc3{ nullptr };
		};
	} // namespace ml
} // namespace forge
