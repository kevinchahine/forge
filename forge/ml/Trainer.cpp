#include "forge/ml/CheckpointManager.h"
#include "forge/ml/trainer.h"
#include "forge/ml/network.h"

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
		template<typename DataLoader>
		void trainEpoch(
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

				for (size_t reuse = 0; reuse < 2; reuse++) {
					//cout << "Resuse: " << reuse << endl;
					// --- Forward Pass ---
					forwardTime.resume();
					auto output = network->forward(data);
					forwardTime.pause();

					//cout << "pred: " << output[0].item template<float>() << " target: " << targets[0].item template<float>() << endl;

					// --- Calc Loss --- 
					lossTime.resume();
					//cout << output.sizes() << '\t' << targets.sizes() << endl;
					auto loss = torch::mse_loss(output, targets);
					//cout << "loss: " << loss.sizes() << " " << loss << endl;
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
							<< setprecision(3) << float(nSamples) / data_size << "% of dataset"
							<< "\tTotal Loss:         " << totalLoss / nSamples
							<< "\tBatch Loss:         " << loss.template item<float>() / output.sizes()[0] << endl
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

						printTimer.expires_from_now(chrono::seconds(8));
						printTimer.resume();
						totalTime.resume();
					}
				} // reuse loop

				getTime.resume();
			}
		}

		// --------------------------------------------------------------------

		void Trainer::train() {
			// --- Load Data ---
			filesystem::path path = R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData.csv)dil";

			cout << "Preparing dataset" << endl;
			auto sf_dataset = StockfishDataset();
			sf_dataset.open(path);
			sf_dataset.skip(0);

			cout << "Mapping dataset" << endl;
			auto dataset = sf_dataset.map(torch::data::transforms::Stack<>());
			auto size = dataset.size().value();

			cout << "Preparing loader" << endl;
			auto loader =
				torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
					std::move(dataset), 5'000);

			CheckpointManager checkpointManager;
			checkpointManager.checkpointDir(g_checkpointDir);
			checkpointManager.name("networkA");

			// --- Training Loop ---

			Network network;

			filesystem::path latest = checkpointManager.latest();
			if (filesystem::exists(latest)) {
				cout << latest.generic_string() << " exists. Loading checkpoint...";
				torch::load(network, latest.generic_string());
				cout << "done" << endl;
			}
			else {
				cout << "Uhho " << latest.generic_string() << " does not exist" << endl;
			}

			network->to(g_computingDevice);

			torch::optim::Adam optimizer(
				network->parameters(), torch::optim::AdamOptions(0.002));

			for (size_t epoch = 0; epoch < 10; epoch++) {
				trainEpoch(network, *loader, optimizer, epoch + 1, size, checkpointManager);
				cout << endl;
			}
		}

	} // namespace ml
} // namespace forge