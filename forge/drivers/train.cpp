#include "train.h"

#include "forge/ml/Trainer.h"

using namespace std;

namespace forge
{
	namespace drivers
	{
		void train_nn() {
			forge::ml::Trainer trainer;

			//trainer.checkpointDir();
			trainer.train();
		}

		void test_nn() {
			forge::ml::Trainer trainer;

			trainer.test();
		}
	} // namespace drivers
} // namespace forge

