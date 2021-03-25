#include "KeyboardController.h"

namespace forge
{
	void KeyboardController::reset()
	{
		currMove = Move();
	}

	Move KeyboardController::getMove(const Position & position)
	{
		cin >> currMove;

		// Consider validating move before returning.
		// Consider PGN moves here since we have the Position

		return currMove;
	}

	string KeyboardController::getName()
	{
		return typeid(*this).name();
	}
} // namespace forge