/*
------- FORGE ---------
*** Forged by Fire ***
A Chess engine designed to reach an elo of atleast 1400.
Designed to use a variety of search algorithms including:
	- negamax (minimax)
	- monte carlo tree search (mcts)

Designed to use a variety of evaluation functions including:
	- traditional chess engine evaluation functions using:
		- Linear combinations
		- Neural Networks
	- deep neural network (DNN)

Designed to be compatible with a variety of displays including:
	- Universal Chess Interface (UCI) GUIs
	- Custom command line GUI for systems that don't support graphical output.
*/

#include "Test.h"	// Contains allToFen necessary includes
#include "TestMoveGen.h"

#include <set> // remove

using namespace std;

int main(int argc, char ** argv)
{
	cout << guten::color::red << "=============\n"
		<< guten::color::yellow << "=== " << guten::color::lightred << "FORGE "
		<< guten::color::yellow << "===\n"
		<< guten::color::red << "=============\n"
		<< guten::color::white;
	
	forge::test::initTestBoards();
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
	//forge::test::placeMethods();
	//forge::test::piece_moves::king();
	//forge::test::piece_moves::queen();
	//forge::test::piece_moves::bishop();
	//forge::test::piece_moves::knight();
	//forge::test::piece_moves::rook();
	//forge::test::piece_moves::pawn();
	//forge::test::fen::allToFen();
	forge::test::fen::allFromFen();
	//forge::test::pins::isPinPossible();
	//forge::test::pins::absolutePins();
	//forge::test::movegen::genThreatBoard();
	//forge::test::movegen::genAttackRay();
	//forge::test::movegen::findKingAttackers();
	//forge::test::movegen::genKingMoves();
	//forge::test::moveKings();
	//forge::test::legalMoveGenerator();
	//forge::test::isAttacked();
	//forge::test::move();
	//forge::test::chessMatch();
	//forge::test::performanceTester();
	//forge::test::nodeIterator();
	//forge::test::heuristic();
	//forge::test::solver();
	//forge::test::insufficientMaterial();
	//forge::test::gameState();
	//forge::test::drawByRepetition();
	//forge::test::logger();
	//forge::test::weights::applePie();
	//forge::test::ai::playApplePie();

	// Rename stockfish and forge
	//forge::test::movegen::stockfishAndForge();

	// 1nbB2qr/rp2k3/2nbp3/1N3p1p/P2p3P/R2BPp2/1PP2KP1/4QR2 b - - 2 26
	// 8/1P6/7k/b7/2n5/8/8/3K4 w - - 1 101

	cout << "Done...";
	this_thread::sleep_for(chrono::seconds(1));
	cin.get();
	return 0;
}