/*
------- FORGE ---------
*** Forged by Fire ***
A Chess engine designed to reach an elo of atleast 1400.
Designed to use a variety of search algorithms including:
	- negamax (minimax)
	- monte carlo tree search (mcts)

Designed to use a variety of evaluation functions including:
	- traditional chess engine evaluation functions
	- deep neural network (DNN)

Designed to be compatible with a variety of displays including:
	- Universal Chess Interface (UCI) GUIs
	- Custom command line GUI for systems that don't support graphical output.

*/

#include "Test.h"	// Contains all necessary includes

using namespace std;

int main(int argc, char ** argv)
{
	cout << guten::color::red << "=============\n"
		<< guten::color::yellow << "=== " << guten::color::lightred << "FORGE "
		<< guten::color::yellow << "===\n"
		<< guten::color::red << "=============\n"
		<< guten::color::white;
	
	//forge::test::clock();
	//forge::test::stopWatch();
	//forge::test::boardDefualt();
	//forge::test::board();
	//forge::test::placeMethods();
	forge::test::legalMoveGenerator();
	//forge::test::isAttacked();
	//forge::test::move();
	//forge::test::chessMatch();
	//forge::test::nodeIterator();
	//forge::test::heuristic();
	//forge::test::solver();
	//forge::test::gameState();

	//this_thread::sleep_for(chrono::seconds(1));
	cin.get();
	return 0;
}