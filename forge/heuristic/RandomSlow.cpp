#include "forge/heuristic/RandomSlow.h"

#include "forge/source/globals.h"

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t RandomSlow::eval(const Position& pos)
		{
			heuristic_t eval = this->super_t::eval(pos);

			// Spin as if thread was executing
			chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
			chrono::high_resolution_clock::time_point now = start;
			while (now - start < this->m_delay) {
				now = chrono::high_resolution_clock::now();
			}

			return eval;
		}
	} // namespace heuristics
} // namespace forge

