#pragma once

#include "../forge/DataSet.h"
#include "../FeatureExtractor.h"

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
			Net(const torch::Device & computingDevice) {
				size_t inputLayerSize = 
					FeatureExtractor::MATERIAL_FEATURES_SIZE +
					FeatureExtractor::ATTACKED_FEATURES_SIZE;

				fc1 = register_module("fc1", torch::nn::Linear(inputLayerSize, 1600));
				fc2 = register_module("fc2", torch::nn::Linear(1600, 1600));
				fc3 = register_module("fc3", torch::nn::Linear(1600, 1));
				
				// These lines are very important to make sure layers are in the correct device
				// operations will hang if these arn't called 
				fc1->to(computingDevice);
				fc2->to(computingDevice);
				fc3->to(computingDevice);
			}
			Net(const Net&) = default;
			Net(Net&&) noexcept = default;
			~Net() noexcept = default;
			Net& operator=(const Net&) = default;
			Net& operator=(Net&&) noexcept = default;

			torch::Tensor forward(torch::Tensor x) {
				// --- Layer 1 ---
				torch::Tensor t = fc1->forward(x);
				t = torch::relu(t);
				t = torch::dropout(t, 0.5, is_training());

				// --- Layer 2 ---
				t = fc2->forward(t);
				t = torch::relu(t);
				
				// --- Layer 3 ---
				t = fc3->forward(t);

				return t;
			}

			void train(DataSet& trainingDS, size_t nEpochs);

		public:
			torch::nn::Linear fc1{ nullptr };
			torch::nn::Linear fc2{ nullptr };
			torch::nn::Linear fc3{ nullptr };
		};
	} // namespace ml
} // namespace forge
