#pragma once

#include <numeric>

#include "forge/heuristic/Base.h"

namespace forge
{
	namespace UCB
	{
		// Evaluation to be used for a position in a winning state
		// Units: Centi Pawns
		const heuristic_t WINNING_EVAL = 1'500;
		
		// Initial value before evaluation
		const heuristic_t INIT = std::numeric_limits<heuristic_t>::max();

		const float MARK_LAST_VISIT = -std::numeric_limits<float>::infinity();

		// UCB = t/n + C * sqrt(ln(N) / n)
		// 	t - total score in units of (win count) (should not be +/- infinity)
		// 	N - Parent node visits
		// 	n - Current node visits
		//	mark - 0 or -inf
		// returns UCB score. Guarenteed to be a real number not +/-infinity or undefined.
		inline float calcUCB(float total, float parentVisits, float currVisits, float mark, float temperature) {
			// UCB = x_i + C * sqrt(ln(N) / n_i)
			// !!! Warning: parentVisits and currVisits must be greater than 0.
			//	log(0) = -NaN (undefined)
			//	x/0	= NaN (undefined)

			return
				(total / (currVisits + 1.0f)) +
				temperature * sqrt(log(parentVisits + 1.0f) / (currVisits + 1.0f)) +
				mark;
		}

		// Map evaluation to range of [-1.0, +1.0]
		// ex: from range [-1500.0, +1500.0] to range [-1.0, +1.0]
		inline float mapRange(heuristic_t eval) {
			if (eval < -WINNING_EVAL)return -WINNING_EVAL;
			else if (eval > WINNING_EVAL)return WINNING_EVAL;
			else return (1.0f / WINNING_EVAL) * eval;
		}
	};
} // namespace forge