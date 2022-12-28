// https://pytorch.org/cppdocs/frontend.html
// https://github.com/pytorch/examples/tree/main/cpp
// 
// To save and load
// https://discuss.pytorch.org/t/libtorch-save-mnist-c-examples-trained-model-into-a-file-and-load-in-from-another-c-file-to-use-for-prediction/51681/9	

#include <iostream>

#include <torch/torch.h>

#include "forge/heuristic/FeatureExtractor.h"

namespace forge
{
	namespace ml
	{
	} // namespace ml
} // namespace forge

class NetworkImpl : public torch::nn::SequentialImpl
{
public:
	NetworkImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1024));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(1024, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 1));
	}

private:
};
TORCH_MODULE(Network);


