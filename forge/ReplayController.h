#pragma once

#include "ControllerBase.h"

#include <queue>

namespace forge
{
	// Makes decisions based on a preconstructed list of Moves in the order they where entered
	// If a move is illegal, it will return the 1st legal move that it finds.
	// ex:
	//	unique_ptr<ReplayController> r;
	//	r.push(Move{ ... });
	//	r.push(Move{ ... });
	//	r.push(Move{ ... });
	//
	// ChessMatch match;
	// match.set
	// Its recommended when playing two ReplayControllers against each other
	//	to assign white's moves to one ReplayController object and black's
	//	moves to another.
	class ReplayController : public ControllerBase, public std::queue<Move>
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position & position)  override;

		virtual std::string getName()  override;

		void pauseBeforeReturn(bool pause = true) { m_pauseBeforeReturning = pause; }

	private:
		bool m_pauseBeforeReturning = false;
	};
} // namespace forge