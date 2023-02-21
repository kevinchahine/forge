#include "play.h"

#include "forge/game/ChessMatch.h"

#include "forge/controllers/Solvers.hpp"
#include "forge/heuristic/Heuristics.hpp"

#include "forge/views/TextView.h"

using namespace std;

namespace forge
{
	namespace drivers
	{
		void play_chess_match() {
			forge::ChessMatch match;

			match.reset();

			match.clock().synchronize(
				std::chrono::minutes(5),
				std::chrono::minutes(5),
				std::chrono::seconds(2),
				std::chrono::seconds(2)
			); // Clock is still paused

			auto whiteController =
				//make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();
				make_unique<MCTS_Sequential>();
				//make_unique<MCTS_Concurrent>();
				//make_unique<MCTS_ProducerConsumer>();
				//make_unique<KeyboardController>();

			auto blackController =
				//make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();
				make_unique<MCTS_Sequential>();
				//make_unique<MCTS_Concurrent>();
				//make_unique<MCTS_ProducerConsumer>();
				//make_unique<KeyboardController>();

			string networkFile = R"dil("C:\Users\kchah\ownCloud\ML\forge\checkpoints\networkC\networkC_00001.pt")dil";

			whiteController->makeHeuristic<
				forge::heuristic::Material>();
				//forge::heuristic::Random>();
				//forge::heuristic::RandomSlow>();
				//forge::heuristic::Rollout>();
				//forge::heuristic::NeuralNetwork>(networkFile);
				//forge::heuristic::NeuralNetwork>();

			blackController->makeHeuristic<
				forge::heuristic::Material>();
				//forge::heuristic::Random>();
				//forge::heuristic::RandomSlow>();
				//forge::heuristic::Rollout>();
				//forge::heuristic::NeuralNetwork>(networkFile);
				//forge::heuristic::NeuralNetwork>();

			whiteController->searchMonitor().timeLimit = std::chrono::seconds(10);
			blackController->searchMonitor().timeLimit = std::chrono::seconds(10);

			//int nodeLimit = 6'000'000;
			//whiteController->searchMonitor().nodeLimit = nodeLimit;
			//blackController->searchMonitor().nodeLimit = nodeLimit;

			//whiteController->threadCount() = 1;// 16;
			//blackController->threadCount() = 1;// 16;

			match.whiteController() = std::move(whiteController);
			match.blackController() = std::move(blackController);

			match.makeView<forge::TextView>();
			match.runGame();
		}
	} // namespace drivers
} // namespace forge


