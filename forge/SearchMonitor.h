#pragma once

#include "Timer.h"
#include "StopWatch.h"

#include <stdint.h>
#include <chrono>

namespace forge
{
	/*
		- number of nodes searched so far 
		- selective search depth in plys
		- search depth in plys 
		- time searched
		- nodes per second searched 
		- pv (the best line found)
		- multi pv (the <k> best lines found; only in multi pv mode)
		- score (the engines approximated score in centi pawns)
		- currently searching move 
		- currently searching move number (1 is 1st move)
		- current line searching (along with what CPU is searching it.)
		- and some other stuff see "engine-interface.txt"
	*/
	class SearchMonitor
	{
	public:
		
		void reset();

		// Call at immediately before the beginning of a search.
		// Keeps track of start time 
		void start();

		bool exitConditionReached() const {
			return
				(nodeCount >= nodeLimit) &&
				(plyCount >= plyLimit) &&
				(timer.is_expired()) &&
				(stopFlag == true);
		}

		double nodesPerSecond() { 
			return
				static_cast<double>(nodeCount) /
				std::chrono::duration<double, std::ratio<1, 1>>(searchTime.elapsed()).count();
		}

		double plysPerSecond() {
			return
				static_cast<double>(plyCount) /
				std::chrono::duration<double, std::ratio<1, 1>>(searchTime.elapsed()).count();
		}

	public:
		int nodeCount = 0;
		int nodeLimit = 0;

		int plyCount = 0;
		int plyLimit = 0;

		StopWatch searchTime;
		Timer timer;

		bool stopFlag = false;
	};
} // namespace forge