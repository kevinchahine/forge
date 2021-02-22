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

#include <iostream>
#include <thread>
#include <chrono>

#include <Guten/guten.hpp>

using namespace std;

int main(int argc, char ** argv)
{
	cout << guten::color::red << "=============\n"
		<< guten::color::yellow << "=== " << guten::color::lightred << "FORGE "
		<< guten::color::yellow << "===\n"
		<< guten::color::red << "=============\n";



	this_thread::sleep_for(chrono::seconds(2));
	//cin.get();
	return 0;
}