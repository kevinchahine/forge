#include "forge/controllers/ReplayController.h"

#include <sstream>
#include <algorithm>

using namespace std;

namespace forge
{
	void ReplayController::reset()
	{
		game_history& super = gameHistory();

		// Make sure that history starts with atleast some starting position
		if (super.empty()) {
			super.emplace_back();
			super.back().position.setupNewGame();
		}

		// Reset iterator to 1 before the first move
		it = this->gameHistory().begin();
	}

	MovePositionPair ReplayController::getMove(const Position & position)
	{
		game_history& super = this->gameHistory();
		
		Move nextMove;

		it++;
		
		if (it != this->gameHistory().end()) {
			nextMove = it->move;
		}
		else {
			cout << "Error: ReplayController: We reached the last move" << endl;
		}

		MoveGenerator2 movegen;
		MoveList legals = movegen.generate(position);

		MoveList::const_iterator itLegals = legals.find(nextMove);

		if (m_pauseBeforeReturning) {
			cout << "Press any key...";
			cin.get();
		}

		// Check if move was legal
		if (itLegals != legals.end()) {
			cout << "Found: " << itLegals->move << '\n';
			return *itLegals;
		}
		else {
			cout << "Move " << nextMove << " is illegal" << endl;
			nextMove.setInvalid();
			return MovePositionPair{ nextMove, position };
		}
	}
	
	void ReplayController::fromHistory(const game_history& history, bool isWhite)
	{
		this->reset();

		game_history& thisGameHistory = this->gameHistory();

		//auto myInserter = inserter(thisGameHistory, thisGameHistory.end());

		//copy(history.begin(), history.end(), myInserter);
	}

	std::string ReplayController::toString() const
	{
		std::stringstream ss;

		// Iterate game history
		for (const auto movePos : *this) {
			ss << movePos << endl;
		}
		
		return ss.str();
	}
} // namespace forge