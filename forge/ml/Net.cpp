#include "Net.h"

#include "../globals.h"
#include "../Timer.h"

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
			cout << "--- Net::train() ---" << endl;
			
			// Move network to the training device (will be a no-op if already there)
			
			int counter = 0;
			chrono::duration printPeriod = chrono::seconds(1);
			Timer timer;
			timer.expires_from_now(printPeriod);
			timer.resume();

			chrono::duration savePeriod = chrono::minutes(1);
			Timer saveTimer;
			saveTimer.expires_from_now(savePeriod);
			saveTimer.resume();

			// TODO: This might need to be put on the GPU. Maybe
			torch::optim::Adam optimizer(this->parameters(), 0.01);
			
			// Repeat until we run out of data
			size_t epoch = 0;
			torch::Tensor loss;
			do {//while (epoch < nEpochs) {
				//cout << "Reading next batch...";
				chrono::time_point start = chrono::high_resolution_clock::now();
				TensorPair batch = trainingDS.getNextBatch();
				batch.moveTo(g_computingDevice);
				//cout << "Moving TensorPair to GPU...";
				//cout << "done " << batch.inputs.device() << '\t' << batch.outputs.device() << endl;

				chrono::time_point stop = chrono::high_resolution_clock::now();
				//cout << "done. took " << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
				//	<< "ms" << endl;


				// When we reach the end of the dataset, start over until we reach the last epoch
				if (batch.nSamples() == 0) {
					trainingDS.reset();
				}

				for (size_t reuse = 0; reuse < 100; reuse++) {
					start = chrono::high_resolution_clock::now();

					// Reset gradients
					optimizer.zero_grad();

					// Execute the model on the input data
					torch::Tensor prediction = this->forward(batch.inputs);

					// Compute a loss value to judge the prediction of our model.
					loss = torch::mse_loss(prediction, batch.outputs);

					// Compute gradients of the loss w.r.t. the parameters of our model
					loss.backward();

					// Update the parameters based on the calculated gradients
					optimizer.step();

					stop = chrono::high_resolution_clock::now();

					//cout << "Epoch duration = " << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
					//	<< "ms " << endl;

					// Output the loss and checkpoint every 100 batches
					if (timer.is_expired()) {
						cout << "Epoch: " << epoch << " | Loss: " << loss.item<float>() << endl;

						timer.expires_from_now(printPeriod);
						timer.resume();
					}

					if (saveTimer.is_expired()) {
						//torch::save(*this, "net.pt");
						saveTimer.expires_from_now(savePeriod);
						saveTimer.resume();
					}

					epoch++;
				}
			} while (loss.item<float>() > 1000);

			// Save model
			// TODO: Make this work. * See above 
			//this->save("trained_models/trained_nn.xml");
		}
	} // namespace ml
} // namespace forge