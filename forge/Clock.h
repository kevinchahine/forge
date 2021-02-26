#pragma once

#include "Timer.h"

#include <iostream>
#include <chrono>

namespace forge
{
	class Clock
	{
	public:
		void start();

		void stop();

		void resetAll();

		void synchronize(
			std::chrono::high_resolution_clock::duration whitesTime,
			std::chrono::high_resolution_clock::duration blacksTime,
			std::chrono::high_resolution_clock::duration whiteInc,
			std::chrono::high_resolution_clock::duration blackInc);

		void click();

		const Timer & getWhiteTimer() const { return whiteTimer; }
		const Timer & getBlackTimer() const { return blackTimer; }

		bool isWhitesTurn() const;
		bool isBlacksTurn() const;

		int getMoveNumber() const;

		//friend std::ostream & operator<<(std::ostream & os, const Clock & clock);

	protected:
		enum class STATE : bool {
			START,		// The clock is ticking
			STOP		// The clock is not ticking
		} currState = STATE::STOP;

		Timer whiteTimer;
		Timer blackTimer;
		std::chrono::milliseconds	whitesIncrement = std::chrono::milliseconds(0);
		std::chrono::milliseconds	blacksIncrement = std::chrono::milliseconds(0);

		// 0 first game state?? So always a fresh board where no pieces have moved??
		// 1 is game state once white plays 1st move??
		// 2 is game state once black plays 1st move??
		int							moveNumber = 0;
	};
} // namespace forge

std::ostream & operator<<(std::ostream & os, const forge::Clock & clock);