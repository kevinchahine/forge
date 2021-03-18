#include "RandomHeuristic.h"

#include "globals.h"

using namespace std;

namespace forge
{
	heuristic_t RandomHeuristic::eval(const Position & pos)
	{
		return mydist(g_rand);	// just return a random number
	}

	unique_ptr<HeuristicBase> RandomHeuristic::clone() const
	{
		return make_unique<RandomHeuristic>(*this);
	}
} // namespace forge