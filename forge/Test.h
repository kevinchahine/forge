#pragma once

#include "Board.h"
#include "Clock.h"
#include "Move.h"

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

		void boardDefualt();

		void board();

		void placeMethods();

		void psuedoValidMoveGenerator();

		void move();

	} // namespace test
} // namespace forge