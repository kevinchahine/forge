#pragma once

#include "Timer.h"

#include <iostream>
#include <chrono>

namespace forge
{
	/*
	ex:
		#include <chrono>
		using namespace std;

		Clock c;							// clock is paused
		c.stop();							// optional: clock is already paused

		c.syncronize(chrono::minutes(15),	// still paused
					 chrono::minutes(15),
					 chrono::seconds(5),
					 chrono::seconds(5));

		c.click();							// 1st click, whites time starts ticking

		while (c.getWhiteTimer().is_not_expired() &&
			   c.getBlackTimer().is_not_expired()) 
		{
			// White makes a move

			c.click();						// whites time stops, blacks time starts ticking

			// Black makes a move				

			c.click();						// blacks time stops, whites time starts ticking
		}

		c.stop();

		cout << (c.getWhiteTimer().is_expired() ? "White" : "Black") << " ran out of time.\n";

	TODO: Add presets like "blitz0", "blitz1", "rapid0", etc
	*/
	class Clock
	{
	public:
		// Stop or pauses timers. Timers will resume apon calling click();
		void stop();

		void resume();

		// Sets allToFen time controls and move count to zero.
		// Sets state to stop. Good to call synchronize after resetAll().
		void resetAll();

		// Used to set each time control or syncronize with a UCI GUI.
		// Does not start or stop the timer.
		void synchronize(
			std::chrono::high_resolution_clock::duration whitesTime,
			std::chrono::high_resolution_clock::duration blacksTime,
			std::chrono::high_resolution_clock::duration whiteInc,
			std::chrono::high_resolution_clock::duration blackInc);

		// Pauses current players timer, and resumes next players timer.
		// 1st click after resetAll() or construction will make white's timer tick.
		// 2nd click after resetAll() or construction will make white's timer pause and black's timer tick.
		void click();

		const Timer & getWhiteTimer() const { return whiteTimer; }
		const Timer & getBlackTimer() const { return blackTimer; }

		bool isWhitesTurn() const;
		bool isBlacksTurn() const;

		int getMoveNumber() const;

		// TODO: implement this method
		//void onTimeout(std::function<void()> && timoutEvent);

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

		// 0 first game state. So always a fresh board where no pieces have moved.
		// 1 is game state once white plays 1st move and black is thinking.
		// 2 is game state once black plays 1st move and white is thinking.
		int							moveNumber = 0;
	};
} // namespace forge

std::ostream & operator<<(std::ostream & os, const forge::Clock & clock);