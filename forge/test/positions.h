#pragma once

#include "forge/core/Position.h"

#include <iostream>
#include <vector>

namespace forge
{
	namespace test
	{
		extern std::vector<Position> g_positions;

		void init();
	} // namespace test
} // namespace forge