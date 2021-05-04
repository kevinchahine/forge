#pragma once

#include "globals.h"

#include "AttackChecker.h"
#include "Board.h"
#include "Clock.h"
#include "StopWatch.h"
#include "Move.h"
#include "ChessMatch.h"
#include "MoveGenerator.h"
#include "GameState.h"
#include "Node.h"

#include "Controllers.hpp"
#include "Heuristics.hpp"
#include "Weights.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include <Guten/guten.hpp>

#include <conio.h>	// Only for testing (Only Windows compatible)

namespace forge
{
	namespace test
	{
		void boardSquare();

		void keyboardController();

		void clock();

		void stopWatch();

		void boardDefualt();

		void board();

		void placeMethods();

		void moveKings();

		void legalMoveGenerator();

		void isAttacked();

		void move();

		void randomSolver();

		void chessMatch();

		void nodeIterator();

		void heuristic();

		void solver();

		void insufficientMaterial();

		void gameState();

		void drawByRepetition();

		namespace weights
		{
			void applePie();
		} // namespace weights
	} // namespace test
} // namespace forge