#include "ApplePieHeuristic.h"

using namespace std;

namespace forge
{
	heuristic_t ApplePieHeuristic::eval(const Position & pos)
	{
		return 0;
	}

	unique_ptr<HeuristicBase> ApplePieHeuristic::clone() const
	{
		return make_unique<ApplePieHeuristic>(*this);
	}
} // namespace forge