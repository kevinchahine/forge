#pragma once

#include "forge/time/Timer.h"
#include "forge/time/StopWatch.h"
#include "forge/util/formatted_int.h"

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
		AC should either be boost::atomic, std::atomic, or forge::composite
	*/
	template< template <typename T, typename... > class AC, typename... Args >
	class SearchMonitorTemplate
	{
	public:

		// Call at immediately before the beginning of a search.
		// Keeps track of start time 
		void start() {
			nodeCount = 0;
			plyCount = 0;
			stopFlag = false;
			searchTime.reset();
			searchTime.resume();
			timer.expires_from_now(timeLimit);
			timer.resume();	// TODO: timer needs to be reset to desired search time also
		}

		// Call immediately after search is completed or terminated.
		// If not called, then timer will continue to accumulate time as if 
		// search is still progressing.
		void stop()	{
			searchTime.pause();
			timer.pause();
		}

		bool exitConditionReached() const {
			return
				(nodeCount >= nodeLimit) ||
				(plyCount >= plyLimit) ||
				(timer.is_expired()) ||	// TODO: is_expired() needs some work
				(stopFlag == true);
		}

		double nodesPerSecond() const {
			return
				static_cast<double>(nodeCount.value()) /
				std::chrono::duration<double, std::ratio<1, 1>>(searchTime.elapsed()).count();
		}

		double plysPerSecond() const {
			return
				static_cast<double>(plyCount.value()) /
				std::chrono::duration<double, std::ratio<1, 1>>(searchTime.elapsed()).count();
		}

		void print(std::ostream& os = std::cout) const
		{
			cout << "Searched:\n"
				<< "\t" << formatted_int(nodeCount.value()) << " nodes\n"
				<< "\t" << nodesPerSecond() << " nodes per second\n"
				<< "\t" << formatted_int(plyCount.value()) << " plys\n"
				<< "\t" << plysPerSecond() << " plys per second\n"
				<< "\t" << "Search time: "
				/******/ << chrono::duration<float, std::ratio<1, 1>>(searchTime.elapsed()).count()
				/******/ << " seconds\n"
				<< '\n';
		}

	public:
		AC<int, Args... > nodeCount = 0;
		AC<int, Args... > nodeLimit = std::numeric_limits<int>::max();

		AC<int, Args... > plyCount = 0;
		AC<int, Args... > plyLimit = std::numeric_limits<int>::max();

		AC<int, Args... > expansionCount = 0;

		// Keeps track of duration of search
		StopWatch searchTime;

		// Times out when desired stop time is reached.
		Timer timer;
		std::chrono::nanoseconds timeLimit = std::chrono::nanoseconds::max();

		// Can be used to terminate search from outside algorithm.
		AC<bool, Args... > stopFlag = false;

		// vvvvvvvvvvvvvvv FOR BENCHMARK TESTING ONLY CAN BE REMOVED VVVVV
		StopWatch selection;
		StopWatch evaluation;
		StopWatch expansion;
		StopWatch backprop;
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	};
} // namespace forge