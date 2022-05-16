#include "forge/controllers/ReplayController.h"

#include <algorithm>

using namespace std;

namespace forge
{
	void ReplayController::reset()
	{
		GameHistory& super = static_cast<GameHistory&>(*this);

		// TODO: Is this good? I think so.
		super.clear();
	}

	MovePositionPair ReplayController::getMove(const Position & position)
	{
		GameHistory& super = static_cast<GameHistory&>(*this);
		
		Move nextMove;

		if (this->size()) {
			nextMove = super.front().move;
			super.pop_back();
		}

		MoveGenerator2 movegen;
		MoveList legals = movegen.generate(position);

		MoveList::const_iterator it = legals.find(nextMove);

		if (m_pauseBeforeReturning) {
			cout << "Press any key...";
			cin.get();
		}

		if (it != legals.end()) {
			cout << "Found: " << it->move << '\n';
			return *it;
		}
		else {
			nextMove.setInvalid();
			return MovePositionPair{ nextMove, position };
		}
	}
	
	void ReplayController::fromHistory(const GameHistory& history, bool isWhite)
	{
		this->reset();

		GameHistory & thisGameHistory = static_cast<GameHistory&>(*this);

		//auto myInserter = inserter(thisGameHistory, thisGameHistory.end());

		//copy(history.begin(), history.end(), myInserter);
	}
} // namespace forge