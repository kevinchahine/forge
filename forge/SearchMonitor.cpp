#include "SearchMonitor.h"

using namespace std;

namespace forge
{
	void SearchMonitor::reset()
	{
		nodeCount = 0;
		//nodeLimit;	// keep the same

		plyCount = 0;
		//plyLimit;		// keep the same
	
		searchTime.reset();
		timer.pause();	// TODO: I don't know

		stopFlag = false;
	}

	void SearchMonitor::start()
	{
		nodeCount = 0;
		plyCount = 0;
		stopFlag = false;
		searchTime.resume();
		timer.resume();
	}
} // namespace forge