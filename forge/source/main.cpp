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

#include "forge/drivers/dataset_helpers.h"
#include "forge/drivers/play.h"
#include "forge/drivers/train.h"

#include "forge/source/globals.h"
#include "forge/source/globals_torch.h"

//#include "forge/test/Benchmark.h"
//#include "forge/test/heuristics.h"
#include "forge/test/mcts.h"
//#include "forge/test/positions.h"
//#include "forge/test/Test.h"
//#include "forge/test/Test.h"
#include "forge/test/tree.h"

// #include "TestFeatureExtractor.h"

#include <guten/color/Color.h>

#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
	cout << termcolor::red << "=============\n"
		<< termcolor::yellow << "=== " << termcolor::bright_red << "FORGE "
		<< termcolor::yellow << "===\n"
		<< termcolor::red << "=============\n"
		<< termcolor::white;

	forge::initGlobals();
	forge::initGlobalsTorch();

	//forge::drivers::split_dataset(90.0f, R"dil(C:/Users/kchah/ownCloud/Datasets/stockfish_evals/chessData.csv)dil");
	forge::drivers::play_chess_match();

	//forge::drivers::train_nn();

	//forge::test::benchmark::mcts();
	//forge::test::benchmark::mcts_phases();
	//forge::test::benchmark::mcts_phases_without_search();

	//forge::test::init();
	//forge::test::initTestBoards();
	
	//forge::test::boardSquare();
	
	//forge::test::bitboard::allShifts();
	//forge::test::bitboard::allMasks();
	
	//forge::test::mcts::producerConsumer();
	
	//forge::test::piece::all();
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
	//forge::test::chessMatch();				// **************
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
	//forge::test::tree::print();

	//forge::test::gameHistory();

	//forge::test::weights::applePie();
	//forge::test::ai::playMaterial();
	//forge::test::optimizer();
	//forge::ml::Optimizer op;
	//op.train();

	//forge::uci::test::engineToGui();
	//forge::uci::test::info();

	//forge::test::feature_extractor::init();
	//forge::test::feature_extractor::findAllAttacked();
	//forge::test::feature_extractor::countAllAttacked();

	//forge::test::ml::heuristic();
	//forge::test::heuristics::nn();

	//forge::test::movegen::stockfishAndForge();

	cout << guten::color::yellow << "Closing...";
	//this_thread::sleep_for(chrono::seconds(1));
	cout << "Press any key..." << endl;
	cin.get();
	return 0;
}