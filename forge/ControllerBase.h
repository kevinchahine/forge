#pragma once

#include "Position.h"

#include <iostream>

namespace forge
{
	class ControllerBase
	{
	public:
		virtual void reset();

		virtual Move getMove(const Position & position) = 0;

	protected:

	};
} // namespace forge