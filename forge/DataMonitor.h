#pragma once

#include "Move.h"

// #include Boost statistics ?

#include <iostream>
#include <chrono>

namespace forge
{
	// Suggestions for multithreading:
	//	Since this data will be updated freqently during search and to minimize
	//	inter-thread interactions, declare a different instance of this class in each thread.
	//	Add up their values when sending data to uci using operator+()
	class DataMonitor
	{
	public:
		// Keeps track of the time search started
		// Call when search begins.
		void markSearchStart();

		DataMonitor operator+(const DataMonitor & rhs);

	private:

		// Start time of the search. Set from within, markSearchStart();
		std::chrono::high_resolution_clock::time_point startTime;

		// Number of nodes searched so far.
		int m_nodesSearched = 0;

		// Number of plys search so far.
		int m_searchDepth = 0;

		///std::queue<Move> m_bestLine; TODO: How should we do this?
		///std::queue<Move> m_currentLineSearching;

		// Engines approximate score in centi pawns
		int m_score = 0;

		// The move that the engine is currently searching.
		Move m_currSearchingMove;


	};
} // namespace forge
