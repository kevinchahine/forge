#include "ViewBase.h"

using namespace std;

namespace forge
{
	void ViewBase::show(const Position & pos, const Move & move)
	{
		cout << __FUNCTION__ << "()\n";
	}

	void ViewBase::show(const Position & pos, const MoveList & validMoves)
	{
		cout << __FUNCTION__ << "()\n";
	}
} // namespace forge