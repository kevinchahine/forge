#include "forge/heuristic/Random.h"

#include "forge/source/globals.h"

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t Random::eval(const Position& pos)
		{
			return mydist(g_rand);
		}
		
		heuristic_t Random::eval(const Position& pos, bool whiteIsSearching)
		{
			// We will break the rules a bit here. Since Random is only
			// intended for testing, we are going to const_cast our random engine.
			return mydist(g_rand);	// just return a random number
		}

		unique_ptr<Base> Random::clone() const
		{
			return make_unique<Random>(*this);
		}

		void Random::print(const Position& pos, std::ostream& os) const
		{
			cout << this->name() << endl;
		}
	} // namespace heuristic
} // namespace forge