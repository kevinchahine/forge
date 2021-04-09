#include "ReplayController.h"

#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	void ReplayController::reset()
	{
		// TODO: Is this good? I think so.
		///while (this->size()) {
		///	this->pop();
		///}
	}

	MovePositionPair ReplayController::getMove(const Position & position)
	{
		Move nextMove;

		if (this->size()) {
			nextMove = this->front();
			this->pop();
		}

		MoveList legals = MoveGenerator::generateLegalMoves(position);

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

	std::string ReplayController::getName()
	{
		return "ReplayController";
	}
} // namespace forge