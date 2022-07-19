#pragma once

#include "forge/controllers/Controllers.hpp"

#include "forge/source/globals.h"

#include <forge/Board.h>
#include <forge/Move.h>

#include "forge/feature_extractor/AttackChecker.h"
#include "forge/feature_extractor/Attackers.h"
#include "forge/feature_extractor/Threats.h"

#include "forge/game/ChessMatch.h"
#include "forge/game/GameState.h"

#include "forge/heuristics/Heuristics.hpp"
#include "forge/heuristics/Weights.hpp"

#include "forge/ml/CSVParser.h"
#include "forge/ml/DataSet.h"
#include "forge/ml/Net.h"

#include <forge/MoveGenerator2.h>

//#include "forge/perft/PerformanceTester.h"
//#include "forge/perft/PerformanceLogger.h"

//#include "forge/search/Node.h"

#include "forge/time/Clock.h"
#include "forge/time/StopWatch.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <guten/guten.hpp>

#ifdef _WIN32		// TODO: Fix this
#include <conio.h>	// Only for testing (Only Windows compatible)
#endif

namespace forge
{
	namespace test
	{
		void initTestBoards();

		void boardSquare();

		namespace bitboard
		{
			void allShifts();

			template<typename DIRECTION_T>
			void showShift(BitBoard bb, uint8_t nCells) {
				Board b;

				std::cout << "=== Shift: " << typeid(DIRECTION_T).name() << " x" << (int)nCells << " ===\n";

				bb.shift<DIRECTION_T>(nCells);

				b.placePieces(bb, pieces::whiteQueen);

				b.printMini();
			}

			void allMasks();

			template<typename DIRECTION_T>
			void showMask(BoardSquare bs) {
				Board b;

				std::cout << "=== Mask: " << typeid(DIRECTION_T).name() << " ===\n";

				BitBoard bits = BitBoard::mask<DIRECTION_T>(bs);

				b.placePieces(bits, pieces::whiteQueen);
				pieces::Piece p = b.at(bs);
				p.makeBlack();
				b.placePiece(bs, p);
				
				b.printMini();
			}

		} // namespace bitboard

		namespace piece
		{
			void allToFen();
		} // namespace pieces

		namespace direction
		{
			void pointingTo();
		}

		void keyboardController();

		void clock();

		void stopWatch();

		void boardDefualt();

		void board();
		
		void boardRotate();

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
				cb.placePiece(piece.getCh(), guten::Point(square.row(), square.col()), true);

				///TextView::highlightCells(bb, termcolor::bright_red, termcolor::red, cb);

				cb.print();
			}

			void king();

			void queen();

			void bishop();

			void knight();

			void rook();

			void pawn();

		} // namespace piece_moves

		namespace fen {
			void allToFen();

			void allFromFen();
		} // namespace fen

		namespace attacked_squares
		{

		} // namespace attacked_squares

		namespace pins {
			void isPinPossible();

			void absolutePins();
		}

		namespace movegen
		{
			void genThreatBoard();

			void genAttackRay();

			void findKingAttackers();

			void genKingMoves();
		} // namespace movegen

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

		void csvParser();

		void dataSetPipeline();

		void trainNN();
		
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