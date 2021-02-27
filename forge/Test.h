#pragma once

#include "Board.h"
#include "Clock.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <Guten/guten.hpp>

#include <conio.h>	// Only for testing

namespace forge
{
	namespace test
	{
		void clock();

		void board();
	} // namespace test
} // namespace forge