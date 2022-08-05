#pragma once

#include "forge/controllers/ControllerBase.h"
#include <forge/core/game_history.h>

namespace forge
{
	// Makes decisions based on a preconstructed list of Moves in the order they where entered
	// If a move is illegal, it will return the 1st legal move that it finds.
	// ex:
	//	ReplayController r;
	//	r.push(Move{ ... });
	//	r.push(Move{ ... });
	//	r.push(Move{ ... });
	//
	// ChessMatch match;
	// match.set
	// Its recommended when playing two ReplayControllers against each other
	//	to assign white's moves to one ReplayController object and black's
	//	moves to another.
	class ReplayController : public ControllerBase, public game_history
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position & position) override;

		virtual std::string getName() const override { return typeid(*this).name(); }

		virtual std::string getNameVariant() const override { return "Basic"; }

		void pauseBeforeReturn(bool pause = true) { m_pauseBeforeReturning = pause; }

		// Stores the sequence of moves played by one of the players in the GameHistory
		// The player whose moves are stored is controlled by isWhite
		void fromHistory(const game_history & history, bool isWhite);

	private:
		bool m_pauseBeforeReturning = false;
	};
} // namespace forge