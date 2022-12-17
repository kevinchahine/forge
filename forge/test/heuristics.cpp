#pragma once

#include "forge/test/heuristics.h"
#include "forge/test/positions.h"
#include "forge/heuristic/NeuralNetwork.h"

using namespace std;

namespace forge
{
	namespace test
	{
		namespace heuristics
		{
			void nn() {
				forge::heuristic::NeuralNetwork nn;

				for (size_t i = 0; i < 100; i++) {
					const Position& pos = g_positions.at(i);

					heuristic_t score = nn.eval(pos);

					pos.board().printMini();

					cout << "Evaluation: " << score << endl;
				}
			}
		} // namespace heuristic
	} // namespace test
} // namespace forge

