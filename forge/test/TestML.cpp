#include "TestML.h"

#include "forge/test/positions.h"

using namespace std;

namespace forge
{
	namespace test
	{
		namespace ml
		{
			void heuristic() {
				heuristic::NeuralNetwork h{ "net.pt" };

				for (const auto& pos : g_positions) {
					heuristic_t eval = h.eval(pos, true);

					pos.board().print();
					cout << (pos.isWhitesTurn() ? "Whites" : "Blacks") << " turn. Eval = " << eval << endl << endl;
				}
			}

			/*
			filesystem::path datasetPath =
			//R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData-head.csv)dil";
			R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData.csv)dil";

			forge::ml::Trainer trainer;
			trainer.train();
			*/
		} // namespace ml
	} // namespace test
} // namespace forge