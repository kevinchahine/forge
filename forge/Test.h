#pragma once

#include "globals.h"

#include "AttackChecker.h"
#include "Board.h"
#include "Clock.h"
#include "StopWatch.h"
#include "Move.h"
#include "ChessMatch.h"
#include "PerformanceTester.h"
#include "PerformanceLogger.h"
#include "MoveGenerator.h"
#include "MoveGenerator2.h"
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

		namespace bitboard
		{
			void allShifts();

			template<typename DIRECTION_T>
			void showShift(BitBoard bb, uint8_t nCells) {
				Board b;

				cout << "=== Shift: " << typeid(DIRECTION_T).name() << " x" << (int)nCells << " ===\n";

				bb.shift<DIRECTION_T>(nCells);

				b.placePieces(bb, pieces::whiteQueen);

				b.printMini();
			}

			void allMasks();

			template<typename DIRECTION_T>
			void showMask(BoardSquare bs) {
				Board b;

				cout << "=== Mask: " << typeid(DIRECTION_T).name() << " ===\n";

				BitBoard bits = BitBoard::mask<DIRECTION_T>(bs);

				b.placePieces(bits, pieces::whiteQueen);
				pieces::Piece p = b.at(bs);
				p.makeBlack();
				b.placePiece(bs, p);
				
				b.printMini();
			}

		} // namespace bitboard

		void keyboardController();

		void clock();

		void stopWatch();

		void boardDefualt();

		void board();

		void placeMethods();

		namespace piece_moves
		{
			template<typename PIECE>
			void templatePiece() {
				PIECE piece;
				//BoardSquare square(4, 5);
				BoardSquare square('b', '2');

				BitBoard bb = piece.pushMask(square);

				guten::boards::CheckerBoard cb;
				cb.lightPiece = guten::color::blue;
				cb.drawBackground();
				cb.placePiece(piece.getCh(), guten::Point(square.row(), square.col()), true);

				TextView::highlightCells(bb, guten::color::lightred, guten::color::red, cb);

				cb.print();
			}

			void king();

			void queen();

			void bishop();

			void knight();

			void rook();

			void pawn();

		} // namespace piece_moves

		namespace attacked_squares
		{

		} // namespace attacked_squares

		namespace pins {
			void isPinPossible();

			void absolutePins();
		}

		void moveKings();

		void legalMoveGenerator();

		void isAttacked();

		void move();

		void randomSolver();

		void chessMatch();

		void performanceTester();

		void nodeIterator();

		void heuristic();

		void solver();

		void insufficientMaterial();

		void gameState();

		void drawByRepetition();

		void logger();

		namespace weights
		{
			void applePie();
		} // namespace weights

		namespace ai
		{
			void playApplePie();
		} // namespace ai
	} // namespace test
} // namespace forge