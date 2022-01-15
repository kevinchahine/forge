#include "Clock.h"

#include <iomanip>
#include <sstream>

#include <Guten/color/iocolor.h>

using namespace std;

namespace forge
{
	///void Clock::start()
	///{
	///	whiteTimer.resume();
	///
	///	blackTimer.pause();
	///
	///	currState = STATE::START;
	///}

	void Clock::stop()
	{
		whiteTimer.pause();

		blackTimer.pause();

		currState = STATE::STOP;
	}

	void Clock::resume()
	{
		if (isWhitesTurn()) {
			whiteTimer.resume();
		}
		else {
			blackTimer.resume();
		}

		currState = STATE::START;
	}
	
	void Clock::resetAll()
	{
		// Reassign allToFen fields with default values
		(*this) = Clock{};
	}
	
	void Clock::synchronize(
		std::chrono::high_resolution_clock::duration whitesTime,
		std::chrono::high_resolution_clock::duration whiteInc,
		std::chrono::high_resolution_clock::duration blacksTime,
		std::chrono::high_resolution_clock::duration blackInc)
	{
		this->whiteTimer.expires_from_now(whitesTime);
		this->whitesIncrement = chrono::duration_cast<chrono::milliseconds>(whitesIncrement);
		this->blackTimer.expires_from_now(blacksTime);
		this->blacksIncrement = chrono::duration_cast<chrono::milliseconds>(blacksIncrement);
	}
	
	void forge::Clock::click()
	{
		currState = STATE::START;

		// Which player was thinking before the clock was clicked?
		if (isWhitesTurn()) {
			// White clicked the clock so white was thinking.
			// Subtract that amount of time from white's time.
			whiteTimer.pause();
			blackTimer.resume();

			// Give white their increment (Fischer time)
			whiteTimer.add_time(whitesIncrement);
		}
		else {
			// Black clicked the clock so black was thinking.
			// Subtract that amount of time from black's time.
			blackTimer.pause();
			whiteTimer.resume();

			// Give black their increment (Fishcer time)
			blackTimer.add_time(blacksIncrement);
		}

		moveNumber++;
	}

	bool Clock::isWhitesTurn() const
	{
		// Even numbers are whites moves: 0, 2, 4, ...
		return moveNumber % 2 == 0;
	}
	
	bool Clock::isBlacksTurn() const
	{
		// Odd numbers are blacks moves: 1, 3, 5, ...
		return !isWhitesTurn();
	}
	
	int Clock::getMoveNumber() const
	{
		return moveNumber;
	}
	
} // namespace forge

ostream & operator<<(ostream & os, const forge::Clock & clock)
{
	chrono::nanoseconds btime = clock.getBlackTimer().expires_from_now();
	chrono::minutes bTimeMin = chrono::duration_cast<chrono::minutes>(btime);
	chrono::seconds bTimeSec = chrono::duration_cast<chrono::seconds>(btime - bTimeMin);

	chrono::nanoseconds wtime = clock.getWhiteTimer().expires_from_now();
	chrono::minutes wTimeMin = chrono::duration_cast<chrono::minutes>(wtime);
	chrono::seconds wTimeSec = chrono::duration_cast<chrono::seconds>(wtime - wTimeMin);
	
	stringstream bss;
	stringstream wss;

	os << "Black: ";

	bss << bTimeMin.count() << ":";
	if (bTimeSec.count() < 10)	bss << '0';
	bss << bTimeSec.count();
	
	os << setw(5) << bss.str();

	os << "      White: ";

	wss << wTimeMin.count() << ":";
	if (wTimeSec.count() < 10) wss << '0';
	wss << wTimeSec.count();

	os << setw(5) << wss.str();

	return os;
}