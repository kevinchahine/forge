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
		class Flatten : public torch::nn::Module
		{
		public:
			static torch::Tensor forward(torch::Tensor t) {
				int64_t nSamples = t.size(0);

				int64_t elements = 1;
				for (int i = 1; i < t.dim(); i++) {
					elements *= t.size(i);
				}

				return t.view({ nSamples, elements });
			}
		};

		class Network : public torch::nn::Module {
		public:
			virtual torch::Tensor forward(torch::Tensor x) = 0;

			virtual std::string name() = 0;
		};

		class NetworkA : public Network
		{
		public:
			NetworkA() {
				l1 = register_module("l1", torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1024));
				l2 = register_module("l2", torch::nn::Linear(1024, 512));
				l3 = register_module("l3", torch::nn::Linear(512, 256));
				l4 = register_module("l4", torch::nn::Linear(256, 1));
			}

			virtual torch::Tensor forward(torch::Tensor x) override {
				x = forge::ml::Flatten::forward(x);
				x = l1->forward(x);
				x = torch::relu(x);
				x = l2->forward(x);
				x = torch::relu(x);
				x = l3->forward(x);
				x = torch::relu(x);
				x = l4->forward(x);

				return x;
			}

			virtual std::string name() override { return "NetworkA"; }

		private:
			torch::nn::Linear l1{ nullptr };
			torch::nn::Linear l2{ nullptr };
			torch::nn::Linear l3{ nullptr };
			torch::nn::Linear l4{ nullptr };
		};
	} // namespace ml
} // namespace forge

class NetworkBImpl : public torch::nn::SequentialImpl
{
public:
	NetworkBImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Flatten(torch::nn::FlattenOptions().start_dim(1).end_dim(4)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 128));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(128, 64));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(64, 1));
	}
};
TORCH_MODULE(NetworkB);

class NetworkCImpl : public torch::nn::SequentialImpl
{
public:
	NetworkCImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 128));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(128, 1));
	}
};
TORCH_MODULE(NetworkC);

class NetworkDImpl : public torch::nn::SequentialImpl
{
public:
	NetworkDImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 1));
	}
};
TORCH_MODULE(NetworkD);

class NetworkEImpl : public torch::nn::SequentialImpl
{
public:
	NetworkEImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 1));
	}
};
TORCH_MODULE(NetworkE);

class NetworkFImpl : public torch::nn::SequentialImpl
{
public:
	NetworkFImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 128));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 128));
		torch::nn::SequentialImpl::push_back(torch::nn::Functional(torch::relu));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(128, 1));
	}
};
TORCH_MODULE(NetworkF);

class NetworkGImpl : public torch::nn::SequentialImpl
{
public:
	NetworkGImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1024));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(1024, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 1));
	}

private:
};
TORCH_MODULE(NetworkG);

class NetworkHImpl : public torch::nn::SequentialImpl
{
public:
	NetworkHImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 16'384));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(16'384, 1024));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(1024, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.1)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 1));
	}

private:
};
TORCH_MODULE(NetworkH);

// TODO: use CNN
class NetworkIImpl : public torch::nn::SequentialImpl
{
public:
	NetworkIImpl() {
		//torch::nn::Conv1d(torch::nn::Conv1dOptions().)
		//torch::nn::SequentialImpl::push_back(torch::nn::functional::conv1d(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 16'384));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 16'384));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.15)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(16'384, 1024));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.15)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(1024, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.15)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 1));
	}
private:
};
TORCH_MODULE(NetworkI);

class NetworkJImpl : public torch::nn::SequentialImpl
{
public:
	NetworkJImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 16'384));
		torch::nn::SequentialImpl::push_back(torch::nn::Sigmoid());
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(16'384, 1024));
		torch::nn::SequentialImpl::push_back(torch::nn::Sigmoid());
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(1024, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.15)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 1));
	}
private:
};
TORCH_MODULE(NetworkJ);

class NetworkKImpl : public torch::nn::SequentialImpl
{
public:
	NetworkKImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(512, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 32));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(32, 1));
	}
private:
};
TORCH_MODULE(NetworkK);

class NetworkLImpl : public torch::nn::SequentialImpl
{
public:
	NetworkLImpl() {
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 512));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 256));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(256, 32));
		torch::nn::SequentialImpl::push_back(torch::nn::LeakyReLU(torch::nn::LeakyReLUOptions().negative_slope(0.2)));
		torch::nn::SequentialImpl::push_back(torch::nn::Linear(32, 1));
	}
private:
};
TORCH_MODULE(NetworkL);