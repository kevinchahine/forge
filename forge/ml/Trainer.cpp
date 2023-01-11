#include "forge/ml/trainer.h"

#include "forge/time/stopwatch.h"
#include "forge/time/timer.h"
#include "forge/source/globals_torch.h"

#include <iostream>
#include <filesystem>

using namespace std;

namespace forge
{
	namespace ml
	{
		// returns total loss of the epoch
		template<typename DataLoader, typename NetworkType>
		float trainEpoch(
			NetworkType & network,
			DataLoader & loader,
			torch::optim::Optimizer & optimizer,
			size_t epoch,
			size_t data_size,
			CheckpointManager & checkpointManager) {
			size_t index = 0;// TODO: remove this if unneeded
			network->train();

			float totalLoss = 0;
			size_t nSamples = 0;
			int batchCount = 0;

			// vvvvvvvvvvvvvvvvvvvvv benchmarking vvvvvv
			forge::StopWatch forwardTime;
			forge::StopWatch optimTime;
			forge::StopWatch lossTime;
			forge::StopWatch getTime;
			forge::StopWatch transferTime;
			forge::StopWatch totalTime;
			forge::Timer printTimer;

			getTime.resume();
			totalTime.resume();
			printTimer.expires_from_now(chrono::seconds(1));
			printTimer.resume();

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

			for (auto & batch : loader) {
				getTime.pause();

				transferTime.resume();
				auto data = batch.data.to(g_computingDevice);
				auto targets = batch.target.to(g_computingDevice);
				transferTime.pause();

				nSamples += data.sizes()[0];
				batchCount++;

				for (size_t reuse = 0; reuse < 1; reuse++) {
					//cout << "Resuse: " << reuse << endl;
					// --- Forward Pass ---

					forwardTime.resume();
					auto output = network->forward(data);
					forwardTime.pause();

					// --- Calc Loss --- 
					lossTime.resume();
					//cout << output.sizes() << '\t' << targets.sizes() << endl;
					auto loss = torch::mse_loss(output, targets);

					//cout << setw(10) << "Pred:" << setw(10) << "Targets:" << endl;
					//for (int s = 0; s < output.sizes()[0]; s++) {
					//	cout << setw(10) << output[s].template item<float>() << setw(10) << targets[s].template item<float>() << endl;
					//}

					//cout << "loss: " << loss << endl;

					if (std::isnan(loss.template item<float>())) {
						cout << "Error: nan" << endl;
						continue;
					}
					totalLoss += loss.template item<float>();
					lossTime.pause();

					// --- Calc Gradient and Optimize ---
					optimTime.resume();
					optimizer.zero_grad();
					loss.backward();
					optimizer.step();
					optimTime.pause();

					if (printTimer.is_expired()) {
						totalTime.pause();
						std::cout
							<< "Train Epoch: " << epoch
							<< " nSamples: " << nSamples << " of " << data_size << ' '
							<< setprecision(3) << 100.0f * float(nSamples) / data_size << "% of trainDataset" << endl
							<< "\tEpoch Time:   " << chrono::duration_cast<chrono::seconds>(totalTime.elapsed()).count() << " sec" << endl
							<< "\tTotal Loss:   " << totalLoss / batchCount << endl
							<< "\tBatch Loss:   " << loss.template item<float>() << endl
							<< "\tgetTime:      " << getTime.elapsed().count() / nSamples << " nsec/sample" << endl
							<< "\tforwardTime:  " << forwardTime.elapsed().count() / nSamples << " nsec/sample" << endl
							<< "\ttransferTime: " << transferTime.elapsed().count() / nSamples << " nsec/sample" << endl
							<< "\tlossTime:     " << lossTime.elapsed().count() / nSamples << " nsec/sample" << endl
							<< "\toptimTime:    " << optimTime.elapsed().count() / nSamples << " nsec/sample" << endl
							<< "\ttotalTime:    " << totalTime.elapsed().count() / nSamples << " nsec/sample" << endl
							;

						filesystem::path file = checkpointManager.generateFilename();

						cout << "saving to " << file.generic_string() << "...";
						torch::save(network, file.generic_string());
						cout << "done" << endl;
						cout << endl;

						printTimer.expires_from_now(chrono::seconds(6/*60*/));
						printTimer.resume();
						totalTime.resume();
					}
				} // reuse loop

				getTime.resume();
			}

			filesystem::path file = checkpointManager.generateFilename(epoch);

			cout << "saving to " << file.generic_string() << "...";
			torch::save(network, file.generic_string());
			cout << "done" << endl;
			cout << endl;

			return totalLoss / batchCount;
		}

		template<typename DataLoader, typename NetworkType>
		float testEpoch(
			NetworkType & network,
			DataLoader & loader) {

			cout << "--- Test Epoch ---" << endl;

			network->train(false);

			float totalLoss = 0;
			size_t nSamples = 0;
			int batchCount = 0;

			for (auto & batch : loader) {
				auto data = batch.data.to(g_computingDevice);
				auto targets = batch.target.to(g_computingDevice);

				nSamples += data.sizes()[0];
				batchCount++;

				auto output = network->forward(data);

				auto loss = torch::mse_loss(output, targets);

				if (std::isnan(loss.template item<float>())) {
					cout << "Error: nan" << endl;
					continue;
				}

				totalLoss += loss.template item<float>();
			}

			cout << "Test Loss: " << totalLoss / batchCount << endl;

			return totalLoss / batchCount;
		}

		// --------------------------------------------------------------------

		void Trainer::train() {
			// --- Load Data ---
			filesystem::path path;

			// --- Training Data ---

			cout << "Preparing Train Dataset" << endl;

			path =
				R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData_train.csv)dil";

			_trainDataset = StockfishDataset();
			_trainDataset.batchSize(100'00);
			_trainDataset.open(path);
			_trainDataset.skip(0);

			// --- Testing Data ---

			cout << "Preparing Test Dataset" << endl;

			path =
				R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData_test.csv)dil";

			_testDataset = StockfishDataset();
			_testDataset.open(path);
			_testDataset.skip(0);

			// --- Checkpoints ---

			_checkpoint.checkpointDir(g_checkpointDir);
			_checkpoint.name(_network);

			// --- Load/Make Network ---

			filesystem::path latest = _checkpoint.latest();
			if (filesystem::exists(latest)) {
				cout << latest.generic_string() << " exists. Loading checkpoint...";
				torch::load(_network, latest.generic_string());
				cout << "done" << endl;
			}
			else {
				cout << "Uhho " << latest.generic_string() << " does not exist" << endl;
			}

			_network->to(g_computingDevice); 
			
			ofstream out;

			filesystem::path lossFile = _checkpoint.networkDir() / "loss.txt";

			// --- Log File ---

			out.open(lossFile.generic_string());
			if (out.is_open()) {
				cout << "Saving loss to: " << lossFile.generic_string() << endl;
			}
			else {
				cout << "Error: Could not open a loss file for writting. " << lossFile.generic_string() << endl;
			}

			out << left
				<< "Training:" << endl
				<< setw(15) << "Epoch:" << setw(15) << "Train Loss: " << setw(15) << "Test Loss: " << endl
				<< flush;

			// --- Training Data ---

			cout << "Mapping Training Dataset" << endl;
			auto trainDataset = _trainDataset.map(torch::data::transforms::Stack<>());
			auto trainSize = trainDataset.size().value();
			int batchSize = 5'000;

			cout << "Preparing Training Data Loader" << endl;
			auto trainLoader =
				torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
					std::move(trainDataset), batchSize);

			cout << "Creating Optimizer" << endl;
			torch::optim::Adam optimizer(
				_network->parameters(), torch::optim::AdamOptions(0.002));

			// --- Testing Data ---

			cout << "Mapping Testing Dataset" << endl;
			auto testDataset = _testDataset.map(torch::data::transforms::Stack<>());
			auto testSize = testDataset.size().value();

			cout << "Preparing Testing Data Loader" << endl;
			auto testLoader =
				torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
					std::move(testDataset), batchSize);

			// --- Training Loop ---

			cout << "--- Training Loop ---" << endl;

			for (size_t epoch = 0; epoch < 20; epoch++) {
				cout << "--- Epoch: " << epoch + 1 << " ---" << endl;
				out << setw(15) << epoch + 1 << ' ' << flush;

				// --- Train Epoch ---
				float epochLoss = trainEpoch(_network, *trainLoader, optimizer, epoch + 1, trainSize, _checkpoint);
				out << setw(15) << epochLoss << ' ' << flush;
				
				// --- Test Epoch ---
				float validationLoss = testEpoch(_network, *testLoader);
				out << setw(15) << validationLoss << ' ' << flush;

				out << endl << flush;
				cout << endl;
			}
		}
	} // namespace ml
} // namespace forge