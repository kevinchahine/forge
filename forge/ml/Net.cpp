#include "Net.h"

#include "forge/source/globals.h"
#include "forge/time/Timer.h"

#include <torch/csrc/api/include/torch/optim/adam.h>
#include <torch/csrc/api/include/torch/serialize.h>
#include <torch/csrc/api/include/torch/cuda.h>

#include <fstream>	// remove this its only temporary

// https://pytorch.org/tutorials/advanced/cpp_frontend.html#writing-the-training-loop	// training in PyTorch (GPU also)

using namespace std;

namespace forge
{
	namespace ml
	{
		void Net::train(DataSet& trainingDS, size_t nEpochs)
		{
			cout << "--- Net::train() ---" << endl;
			
			this->load("net.pt");
			
			ofstream outFile;
			outFile.open("training_history.txt");
			outFile << "epoch, (mse) loss" << endl;

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
			torch::optim::Adam optimizer(this->parameters(), 0.002);
			
			// Repeat until we run out of data
			size_t epoch = 0;
			torch::Tensor loss;
			float lossVal = 0.0f;

			do {//while (epoch < nEpochs) {
				cout << "Reading next batch..." << flush;
				chrono::time_point start = chrono::high_resolution_clock::now();
				TensorPair batch = trainingDS.getNextBatch();
				batch.moveTo(g_computingDevice);
				cout << "done " << batch.inputs.device() << '\t' << batch.outputs.device() << endl;

				chrono::time_point stop = chrono::high_resolution_clock::now();
				cout << "done. took " << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
					<< "ms" << endl;

				// When we reach the end of the dataset, start over until we reach the last epoch
				if (batch.nSamples() == 0 || batch.nSamples() < trainingDS.batchSize() / 2) {
					cout << "End of data set reached. Going back to the beginning" << endl;
					trainingDS.reset();	// go back to the beginning of training file
					continue;			// go to the end of loop and repeat
				}

				for (size_t reuse = 0; reuse < 100; reuse++) {
					// Reset gradients
					optimizer.zero_grad();

					// Execute the model on the input data
					torch::Tensor prediction = this->forward(batch.inputs);

					// Compute a loss value to judge the prediction of our model.
					loss = torch::mse_loss(prediction, batch.outputs);
					loss.cuda();		// ??? Is this right
					loss = loss.cuda();	// ??? or this

					// Compute gradients of the loss w.r.t. the parameters of our model
					loss.backward();

					// Update the parameters based on the calculated gradients
					optimizer.step();

					lossVal = loss.item<float>();
					
					outFile << epoch << ", " << lossVal << '\n';

					// Output the loss and checkpoint periodically
					if (timer.is_expired()) {
						cout << "Epoch: " << epoch << " | Loss: " << lossVal << endl;

						timer.expires_from_now(printPeriod);
						timer.resume();
					}

					if (saveTimer.is_expired()) {
						cout << "Saving checkpoint...";
						this->save("net.pt");
						cout << "done" << endl;
						saveTimer.expires_from_now(savePeriod);
						saveTimer.resume();
					}

					epoch++;
				}
			} while (lossVal > 1000);

			outFile.close();
			// Save model
			// TODO: Make this work. * See above 
			// torch::save(*this, "trained_models/net.pt");
		}

		void Net::save(const string & filename) 
		{
			torch::serialize::OutputArchive outputArchive;

			this->torch::nn::Module::save(outputArchive);	// Call parents method overload 
			outputArchive.save_to(filename);
		}

		void Net::load(const string & filename)
		{
			torch::serialize::InputArchive inputArchive;

			inputArchive.load_from(filename);
			this->torch::nn::Module::load(inputArchive);	// Call parents method overload
		}
		
	} // namespace ml
} // namespace forge