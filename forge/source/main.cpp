/*
------- FORGE ---------
*** Forged by Fire ***
A Chess engine designed to reach an elo of atleast 1400.
Designed to use a variety of decision making algorithms including:
	- negamax (minimax)
	- monte carlo tree search (mcts)
	- random (for testing)

Designed to use a variety of evaluation functions including:
	- traditional chess engine evaluation functions using:
		- Single Layer Perceptrons (SLP) (traditional evaluation functions)
	- deep neural network (DNN)
		- Multilayer Perceptrons (MLP)

Designed to be compatible with a variety of displays including:
	- Universal Chess Interface (UCI) GUIs
	- Terminal interface.
*/

#include "forge/source/globals.h"
#include "forge/test/Test.h"
#include "forge/test/TestML.h"
#include "forge/test/positions.h"
// #include "TestFeatureExtractor.h"
// #include "ml/Optimizer.h"

#include <guten/color/Color.h>

#include <iostream>
#include <forge/feature_extractor/AttackChecker.h>

using namespace std;

int main(int argc, char ** argv)
{
	cout << termcolor::red << "=============\n"
		<< termcolor::yellow << "=== " << termcolor::bright_red << "FORGE "
		<< termcolor::yellow << "===\n"
		<< termcolor::red << "=============\n"
		<< termcolor::white;

	forge::initGlobals();
	//forge::test::initTestBoards();
	//forge::test::init();
	
	//forge::test::boardSquare();
	//forge::test::bitboard::allShifts();
	//forge::test::bitboard::allMasks();
	//forge::test::piece::allToFen();
	//forge::test::direction::pointingTo();
	//forge::test::keyboardController();
	//forge::test::clock();
	//forge::test::stopWatch();
	//forge::test::boardDefualt();
	//forge::test::board();
	//forge::test::boardRotate();
	//forge::test::placeMethods();
	//forge::test::piece_moves::king();
	//forge::test::piece_moves::queen();
	//forge::test::piece_moves::bishop();
	//forge::test::piece_moves::knight();
	//forge::test::piece_moves::rook();
	//forge::test::piece_moves::pawn();
	//forge::test::fen::allToFen();
	//forge::test::fen::allFromFen();
	//forge::test::pins::isPinPossible();
	//forge::test::pins::absolutePins();
	//forge::test::movegen::genThreatBoard();
	//forge::test::movegen::genAttackRay();
	//forge::test::movegen::findKingAttackers();
	//forge::test::movegen::genKingMoves();
	//forge::test::moveKings();
	//forge::test::legalMoveGenerator();	// ***********************************
	//forge::test::isAttacked();
	//forge::test::move();
	forge::test::chessMatch();				// **************
	//forge::test::uciMatch();					// uuuuuuuuuuuuuu
	//forge::test::performanceTester();
	//forge::test::nodeIterator();
	//forge::test::heuristic();
	//forge::test::solver();
	//forge::test::insufficientMaterial();
	//forge::test::gameState();
	//forge::test::drawByRepetition();
	//forge::test::logger();
	//forge::test::csvParser();
	//forge::test::dataSetPipeline();
	//forge::test::trainNN();				// ttttttttttttttttttttttttttttttttt

	//forge::test::weights::applePie();
	//forge::test::ai::playApplePie();
	//forge::test::optimizer();
	//forge::ml::Optimizer op;
	//op.train();

	//forge::uci::test::engineToGui();
	//forge::uci::test::info();

	//forge::test::feature_extractor::init();
	//forge::test::feature_extractor::findAllAttacked();
	//forge::test::feature_extractor::countAllAttacked();

	//forge::test::ml::heuristic();

	//forge::test::movegen::stockfishAndForge();

	//this_thread::sleep_for(chrono::seconds(1));
	//cout << "Press any key..." << endl;
	//cin.get();
	return 0;
}