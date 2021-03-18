#pragma once

#include "globals.h"

#include "Board.h"
#include "Clock.h"
#include "StopWatch.h"
#include "Move.h"
#include "ChessMatch.h"
#include "MoveGenerator.h"
#include "Node.h"

#include "Solvers.hpp"
#include "Heuristics.hpp"

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

		void stopWatch();

		void boardDefualt();

		void board();

		void placeMethods();

		void psuedoValidMoveGenerator();

		void move();

		void randomSolver();

		void chessMatch();

		void nodeIterator();

		void heuristic();

		void solver();

	} // namespace test
} // namespace forge