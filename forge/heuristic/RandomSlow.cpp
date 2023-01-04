#include "forge/heuristic/RandomSlow.h"

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t RandomSlow::eval(const Position& pos)
		{
			return eval(pos, true);
		}
		
		heuristic_t RandomSlow::eval(const Position& pos, bool maximizeWhite)
		{
			heuristic_t eval = this->super_t::eval(pos, maximizeWhite);

			// Spin as if thread was executing
			chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
			chrono::high_resolution_clock::time_point now = start;
			while (now - start < this->m_delay) {
				now = chrono::high_resolution_clock::now();
			}

			return eval;
		}
		
		vector<heuristic_t> RandomSlow::eval(const vector<const Position *> & positions, bool whiteIsSearching) {
			vector<heuristic_t> evals;
			evals.reserve(positions.size());

			for (const Position * p : positions) {
				evals.push_back(this->eval(Position(), whiteIsSearching));
			}

			return evals;
		}
	} // namespace heuristics
} // namespace forge

