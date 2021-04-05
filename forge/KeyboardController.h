#pragma once

#include "ControllerBase.h"

namespace forge
{
	class KeyboardController : public ControllerBase
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position & position) override;

		virtual std::string getName() override;

	private:
		
		// returns true if 'move' is found in 'legalMoves'
		// 'legalMoves' must contain all valid moves for the current player
		// for this method to work properly
		static MoveList::const_iterator findMatchingMove(Move move, const MoveList & legalMoves);

	protected:
		// Initialized to a full move. Initial value is irrelevant
		Move m_move{ BoardSquare{ 'a', '1' }, BoardSquare{ 'a', '2' } };	
	};
} // namespace forge