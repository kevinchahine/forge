#pragma once

#include "forge/Position.h"

#include <iostream>
#include <iomanip>

namespace forge
{
	namespace ml
	{
		class PosEvalPair
		{
		public:
			PosEvalPair() = default;
			PosEvalPair(const PosEvalPair&) = default;
			PosEvalPair(PosEvalPair&&) noexcept = default;
			PosEvalPair(const Position & pos, int eval) : pos(pos),	eval(eval) {}
			~PosEvalPair() noexcept = default;
			PosEvalPair & operator=(const PosEvalPair &) = default;
			PosEvalPair & operator=(PosEvalPair &&) noexcept = default;

			friend std::ostream & operator<<(std::ostream & os, const PosEvalPair & pair) {
				os << pair.pos << ' ' << pair.eval;
				return os;
			}
				
		public:
			Position pos;
			int eval;
		};
	} // namespace ml
} // namespace forge