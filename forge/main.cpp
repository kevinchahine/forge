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

#include "Test.h"	// Contains all necessary includes
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
	//forge::test::piece::all();
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
	forge::test::movegen::lc0AndForge();

	set<forge::Position> setA, setB, setC;

	set_intersection(
		setA.begin(), setA.end(),
		setB.begin(), setB.end(),
		inserter(setC, setC.end()));

	cout << "Done...";
	this_thread::sleep_for(chrono::seconds(1));
	cin.get();
	return 0;
}