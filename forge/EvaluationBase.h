#pragma once

#include "Position.h"

namespace forge
{
	class EvaluationBase
	{
	public:
		virtual double eval(const Position & pos) = 0;

	protected:

	};
} // namespace forge