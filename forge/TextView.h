#pragma once

#include "ViewBase.h"

#include "Guten/CheckerBoard.h"

namespace forge
{
	class TextView : public ViewBase
	{
	public:
		// move - move that brought us to this position.
		// pos - represents the current game.
		///virtual void setPieces(const Move & move, const Position & pos) override;

		virtual void highlight(const BoardSquare & cell) override;

		// move - move that brought us to this position.
		// pos - represents the current game.
		virtual void show(const Move & move, const Position & pos) override;

		// move - move that brought us to this position.
		// pos - represents the current game.
		///virtual void show(const Move & move, const Position & pos) override;

	private:

		guten::boards::CheckerBoard m_img;
	};
} // namespace forge