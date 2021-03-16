#pragma once

#include <iostream>
#include <chrono>

namespace forge
{
	class StopWatch
	{
	public:
		// Sets elapsed time to the one specified and pauses clock
		void reset(std::chrono::nanoseconds elapsedTime = std::chrono::nanoseconds(0));

		void pause();

		void resume();

		bool is_paused() const { return state == STATE::PAUSED; }

		bool is_resumed() const { return state == STATE::RESUMED; }

		std::chrono::nanoseconds elapsed() const;

		friend std::ostream & operator<<(std::ostream & os, const StopWatch & sw);

	private:
		// Time at which the timer was last resumed
		std::chrono::high_resolution_clock::time_point lastResumed;

		// Amount of time accumulated in the StopWatch at the time the watch was last resumed.
		std::chrono::nanoseconds m_elapsed = std::chrono::nanoseconds(0);

		enum class STATE : bool
		{
			PAUSED,
			RESUMED
		} state = STATE::PAUSED;
	};
} // namespace forge

//std::ostream & operator<<(std::ostream & os, const forge::StopWatch & sw);
