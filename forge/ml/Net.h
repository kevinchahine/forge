#pragma once

#include "forge/ml/DataSet.h"
#include "forge/ml/CSVDataSet.h"
#include "forge/heuristics/FeatureExtractor.h"

#include <iostream>
#include <string>

#include <torch/torch.h>
//#include <torch/script.h>	// https://pytorch.org/tutorials/advanced/cpp_export.html
// #include "torch\csrc\api\include\torch\nn\module.h"
// #include "torch\csrc\api\include\torch\nn\modules\linear.h"

namespace forge
{
	namespace ml
	{
		class Net : public torch::nn::Module
		{
		public:
			Net(const torch::Device& computingDevice = torch::kCPU);
			Net(const Net&) = default;
			Net(Net&&) noexcept = default;
			~Net() noexcept = default;
			Net& operator=(const Net&) = default;
			Net& operator=(Net&&) noexcept = default;

			torch::Tensor forward(torch::Tensor x) {
				// --- Layer 1 ---
				torch::Tensor t = fc1->forward(x);
				t = torch::relu(t);
				//t = torch::dropout(t, 0.5, is_training());
				
				// --- Layer 2 ---
				t = fc2->forward(t);
				t = torch::relu(t);
				
				// --- Layer 3 ---
				t = fc3->forward(t);
				t = torch::relu(t);
				
				//// --- Layer 4 ---
				t = fc4->forward(t);
				
				return t;
			}

			void train(DataSet& trainingDS, size_t nEpochs);
			void train(CSVDataSet& trainingDS, size_t nEpochs);

			void save(const std::string & filename);

			void load(const std::string & filename);
			
			void to(const torch::Device & device)
			{
				fc1->to(device);
				fc2->to(device);
				fc3->to(device);
				fc4->to(device);
			}

		public:
			torch::nn::Linear fc1{ nullptr };
			torch::nn::Linear fc2{ nullptr };
			torch::nn::Linear fc3{ nullptr };
			torch::nn::Linear fc4{ nullptr };
		};
	} // namespace ml
} // namespace forge
