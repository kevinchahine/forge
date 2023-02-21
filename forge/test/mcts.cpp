#include "forge/controllers/MCTS_ProducerConsumer.h"

#include "forge/heuristic/Material.h"

using namespace std;

namespace forge
{
	namespace test
	{
		namespace mcts
		{
			void producerConsumer() {
				MCTS_ProducerConsumer agent;
				
				agent.makeHeuristic<heuristic::Material>();

				agent.searchMonitor().timeLimit = chrono::seconds(5000);

				agent.threadCount() = 1;

				MovePositionPair pair = agent.getMove(forge::Position());

				cout << pair << endl;
			}
		} // namespace mcts
	} // namespace test
} // namespace forge