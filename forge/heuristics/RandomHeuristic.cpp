#include "RandomHeuristic.h"

#include "globals.h"

using namespace std;

namespace forge
{
	heuristic_t RandomHeuristic::eval(const Position & pos)
	{
		// We will break the rules a bit here. Since RandomHeuristic is only
		// intended for testing, we are going to const_cast our random engine.
		return mydist(g_rand);	// just return a random number
	}

	unique_ptr<HeuristicBase> RandomHeuristic::clone() const
	{
		return make_unique<RandomHeuristic>(*this);
	}

	void RandomHeuristic::print(const Position & pos, std::ostream & os) const
	{
		cout << this->name() << endl;
	}
} // namespace forge