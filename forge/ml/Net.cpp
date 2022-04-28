#include "Net.h"

#include <torch/csrc/api/include/torch/optim/adam.h>
#include <torch/csrc/api/include/torch/serialize.h>

using namespace std;

namespace forge
{
	namespace ml
	{
		void Net::train(DataSet& trainingDS, size_t nEpochs)
		{
			int counter = 0;

			torch::optim::Adam optimizer(this->parameters(), 0.01);

			// Repeat until we run out of data
			for (size_t epoch = 0; epoch < nEpochs; epoch++) {
				TensorPair batch = trainingDS.getNextBatch();
				
				// When we reach the end of the dataset, start over until we reach the last epoch
				if (batch.nSamples() == 0) {
					trainingDS.reset();
				}

				// Reset gradients
				optimizer.zero_grad();

				// Execute the model on the input data
				torch::Tensor prediction = this->forward(batch.inputs);
				// Compute a loss value to judge the prediction of our model.
				torch::Tensor loss = torch::mse_loss(prediction, batch.outputs);
				// Compute gradients of the loss w.r.t. the parameters of our model
				loss.backward();

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