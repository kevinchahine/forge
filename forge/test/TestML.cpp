#include "TestML.h"

#include "forge/test/positions.h"

using namespace std;

namespace forge
{
	namespace test
	{
		namespace ml
		{
			void heuristic()
			{
				heuristic::NeuralNetwork h{ "net.pt" };

				for (const auto & pos : g_positions) {
					heuristic_t eval = h.eval(pos);
				
					pos.board().print();
					cout << (pos.isWhitesTurn() ? "Whites" : "Blacks") << " turn. Eval = " << eval << endl << endl;
				}
			}
		} // namespace ml
	} // namespace test
} // namespace forge