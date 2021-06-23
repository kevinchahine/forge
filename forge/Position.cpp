#include "Position.h"

using namespace std;

namespace forge
{
	void Position::reset()
	{
		m_board.placeAllPieces();

		m_fiftyMoveRule.reset();

		m_moveCounter.reset();
	}

	void Position::clear()
	{
		m_board.reset();
		
		m_fiftyMoveRule.reset();

		m_moveCounter.reset();
	}
} // namespace forge