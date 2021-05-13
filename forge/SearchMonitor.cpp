#include "SearchMonitor.h"

using namespace std;

namespace forge
{
	void SearchMonitor::start()
	{
		nodeCount = 0;
		plyCount = 0;
		stopFlag = false;
		searchTime.reset();
		searchTime.resume();
		timer.resume();	// TODO: timer needs to be reset to desired search time also
	}

	void SearchMonitor::stop()
	{
		searchTime.pause();
		timer.pause();
	}

	void SearchMonitor::print(std::ostream & os) const
	{
		cout << "Searched:\n"
			<< "\t" << nodeCount << " nodes\n"
			<< "\t" << nodesPerSecond() << " nodes per second\n"
			<< "\t" << plyCount << " plys\n"
			<< "\t" << plysPerSecond() << " plys per second\n"
			<< "\t" << "Search time: "
			/******/<< chrono::duration<float, std::ratio<1, 1>>(searchTime.elapsed()).count()
			/******/<< " seconds\n"
			<< '\n';
	}
} // namespace forge