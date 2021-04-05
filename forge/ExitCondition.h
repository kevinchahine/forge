#pragma once

#include "Timer.h"

#include <stdint.h>
#include <chrono>

/*
Exit Condition :
	- node limit
	- depth(ply) limit
	- time limit
	- stop command(from UCI)
	- or maybe even it decides on its own
	- all leaf node reached
*/

namespace forge
{
	class ExitCondition
	{
	public:

		bool exit() const { 
			return
				(nodeCount >= nodeLimit) &&
				(plyCount >= plyLimit) &&
				(timer.is_expired()) &&
				(stopFlag == true);
		}

		bool keepGoing() const { return !exit(); }

	public:
		int nodeCount = 0;
		int nodeLimit = 0;

		int plyCount = 0;
		int plyLimit = 0;

		Timer timer;
		
		bool stopFlag = false;
	};
} // namespace forge