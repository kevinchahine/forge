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
		template<typename DataLoader>
		float trainEpoch(
			Network& network,
			DataLoader& loader,
			torch::optim::Optimizer& optimizer,
			size_t epoch,
			size_t data_size,
			CheckpointManager& checkpointManager) {
			size_t index = 0;
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

			for (auto& batch : loader) {
				getTime.pause();

				transferTime.resume();
				auto data = batch.data.to(g_computingDevice);
				auto targets = batch.target.to(g_computingDevice);
				transferTime.pause();

				nSamples += data.sizes()[0];
				batchCount++;

				for (size_t reuse = 0; reuse < 32; reuse++) {
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
							<< setprecision(3) << 100.0f * float(nSamples) / data_size << "% of dataset" << endl
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

						printTimer.expires_from_now(chrono::seconds(60));
						printTimer.resume();
						totalTime.resume();
					}
				} // reuse loop

				getTime.resume();
			}

			return totalLoss / batchCount;
		}

		// --------------------------------------------------------------------

		void Trainer::train() {
			this->init();
			ofstream out;
			out.open("loss.txt");
			out << "Training" << endl;
			out << setw(10) << "Epoch:" << setw(10) << "Loss:" << endl;

			cout << "Mapping dataset" << endl;
			auto dataset = _dataset.map(torch::data::transforms::Stack<>());
			auto size = dataset.size().value();
			int batchSize = 5'000;

			cout << "Preparing loader" << endl;
			auto loader =
				torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
					std::move(dataset), batchSize);

			torch::optim::Adam optimizer(
				_network->parameters(), torch::optim::AdamOptions(0.002));

			for (size_t epoch = 0; epoch < 1'000; epoch++) {
				float epochLoss = trainEpoch(_network, *loader, optimizer, epoch + 1, size, _checkpoint);
				out << setw(10) << epoch << setw(10) << epochLoss << endl;
				cout << endl;
			}
		}

		void Trainer::test() {
			this->init();

			cout << "Mapping dataset" << endl;
			auto dataset = _dataset.map(torch::data::transforms::Stack<>());
			auto size = dataset.size().value();

			cout << "Preparing loader" << endl;
			auto loader =
				torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
					std::move(dataset), 5'000);

			float totalLoss = 0;
			size_t nSamples = 0;
			forge::Timer printTimer;

			for (auto& batch : *loader) {
				auto data = batch.data.to(g_computingDevice);
				auto targets = batch.target.to(g_computingDevice);

				nSamples += data.sizes()[0];

				// --- Forward Pass ---
				auto output = _network->forward(data);

				// --- Calc Loss --- 
				auto loss = torch::mse_loss(output, targets);

				if (std::isnan(loss.template item<float>())) {
					cout << "Error: nan" << endl;
					continue;
				}
				totalLoss += loss.template item<float>();

				if (printTimer.is_expired()) {
					std::cout
						<< " nSamples: " << nSamples << " of " << size << ' '
						<< "\t" << setprecision(3) << 100.0f * float(nSamples) / size << "% of dataset" << endl
						<< "\tTotal Loss:         " << totalLoss / nSamples << endl
						<< "\tBatch Loss:         " << loss.template item<float>() / output.sizes()[0] << endl
						;

					printTimer.expires_from_now(chrono::seconds(8));
					printTimer.resume();
				}
			}
		}

		void Trainer::init() {
			// --- Load Data ---
			filesystem::path path = R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData.csv)dil";

			cout << "Preparing dataset" << endl;
			_dataset = StockfishDataset();
			_dataset.open(path);
			_dataset.skip(0);

			_checkpoint.checkpointDir(g_checkpointDir);
			_checkpoint.name("networkA");

			// --- Training Loop ---

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
		}

	} // namespace ml
} // namespace forge