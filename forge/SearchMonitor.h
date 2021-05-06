#pragma once

#include "Timer.h"
#include "StopWatch.h"

#include <stdint.h>
#include <chrono>

namespace forge
{
	/*
	Purpose: Monitors search, saves performance metrics, and determines when to
		end a search.
		Keeps track of:
		- number of nodes searched so far 
		- selective search depth in plys
		- search depth in plys 
		- time searched
			- time started 
			- time stopped
		- nodes per second searched 
		- pv (the best line found so far)
		- multi pv (the <k> best lines found; only in multi pv mode)
		- score (the engines approximated score in centi pawns)
			- This would be in reference to the best line found 
		- currently searching move (Just the 1st ply not the node itself)
		- currently searching move number (1 is 1st legal move, 2 is 2nd legal move)
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
				(nodeCount >= nodeLimit) ||
				(plyCount >= plyLimit) ||
				//(timer.is_expired()) ||	// TODO: is_expired() needs some work
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
		int nodeLimit = std::numeric_limits<int>::max();

		int plyCount = 0;
		int plyLimit = std::numeric_limits<int>::max();

		// Keeps track of duration of search
		StopWatch searchTime;

		// Times out when desired stop time is reached.
		Timer timer;

		// Can be used to terminate search from outside algorithm.
		bool stopFlag = false;
	};
} // namespace forge