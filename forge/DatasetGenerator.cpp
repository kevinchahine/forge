#include "UciEngine.h"

#include "DatasetGenerator.h"

using namespace std;
using namespace forge::uci;

namespace forge
{
	void DatasetGenerator::run()
	{
		uci::UciEngine stockfish;
		stockfish.launch();
		
	}
} // namespace forge