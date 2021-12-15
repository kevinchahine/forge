#include "Timer.h"

using namespace std;

namespace forge
{
	void Timer::pause()
	{
		// Make sure not to pause a paused timer.
		if (state == STATE::RESUMED) {
			// What time is it?
			chrono::high_resolution_clock::time_point currTime = chrono::high_resolution_clock::now();

			// How long has the timer been ticking since it was last resumed?
			chrono::nanoseconds sinceLastResume = (currTime - lastResumed);

			// Decrease that time from expiry
			expiry -= sinceLastResume;

			state = STATE::PAUSED;
		}
	}
	
	void Timer::resume()
	{
		if (state == STATE::PAUSED) {
			lastResumed = chrono::high_resolution_clock::now();

			state = STATE::RESUMED;
		}
	}

	bool Timer::is_expired() const
	{
		return expires_from_now().count() <= 0;
	}

	void Timer::expires_from_now(const chrono::nanoseconds & duration)
	{
		lastResumed = chrono::high_resolution_clock::now();
		
		expiry = duration;
	}

	std::chrono::nanoseconds Timer::expires_from_now() const
	{
		switch (state)
		{
		case STATE::PAUSED:			return expiry;
		case STATE::RESUMED:		return expiry - (chrono::high_resolution_clock::now() - lastResumed);
		}
	}

	void Timer::add_time(const std::chrono::nanoseconds & duration)
	{
		expiry += duration;
	}

} // namespace forge

