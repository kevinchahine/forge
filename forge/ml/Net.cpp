#include "Net.h"

#include <torch/csrc/api/include/torch/optim/adam.h>
#include <torch/csrc/api/include/torch/serialize.h>
#include <torch/csrc/api/include/torch/cuda.h>

// https://pytorch.org/tutorials/advanced/cpp_frontend.html#writing-the-training-loop	// training in PyTorch (GPU also)

using namespace std;

namespace forge
{
	namespace ml
	{
		void Net::train(DataSet& trainingDS, size_t nEpochs)
		{
			// Save device which will perform training
			torch::Device device = torch::kCPU;
			if (torch::cuda::is_available()) {
				std::cout << "CUDA is available! Training on GPU." << std::endl;
				device = torch::kCUDA;
			}

			cout << "device = " << device << endl;

			// Move network to the training device (will be a no-op if already there)
			//this->to(device);
			
			int counter = 0;

			torch::optim::Adam optimizer(this->parameters(), 0.01);

			// Repeat until we run out of data
			for (size_t epoch = 0; epoch < nEpochs; epoch++) {
				TensorPair batch = trainingDS.getNextBatch();
				
				// Move training data to training device
				batch.inputs.to(device);
				batch.outputs.to(device);

				cout << "inputs exists on: " << batch.inputs.device()
					<< " index: " << batch.inputs.get_device() << endl;

				// When we reach the end of the dataset, start over until we reach the last epoch
				if (batch.nSamples() == 0) {
					trainingDS.reset();
				}

				// Reset gradients
				optimizer.zero_grad();

				cout << "forward" << endl;
				// Execute the model on the input data
				torch::Tensor prediction = this->forward(batch.inputs);
				// Compute a loss value to judge the prediction of our model.
				cout << "mse_loss" << endl;
				torch::Tensor loss = torch::mse_loss(prediction, batch.outputs);
				cout << "backward" << endl;
				// Compute gradients of the loss w.r.t. the parameters of our model
				loss.backward();

				cout << "step" << endl;
				// Update the parameters based on the calculated gradients
				optimizer.step();
				
				// Output the loss and checkpoint every 100 batches
				cout << "Epoch: " << epoch << " | Loss: " << loss.item<float>() << endl;
				
				//torch::save(*this, "net.pt");
			}

			// Save model
			// TODO: Make this work. * See above 
			//m_model.save("trained_models/trained_nn.xml");
		}
	} // namespace ml
} // namespace forge