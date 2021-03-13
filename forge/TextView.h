#pragma once

#include "ViewBase.h"

#include "MoveGenerator.h"
#include "Guten/CheckerBoard.h"
#include "MoveList.h"

namespace forge
{
	class TextView : public ViewBase
	{
	public:
		TextView();
		TextView(const TextView &) = default;
		TextView(TextView &&) noexcept = default;
		virtual ~TextView() noexcept = default;
		TextView & operator=(const TextView &) = default;
		TextView & operator=(TextView &&) noexcept = default;

		virtual void show(const Position & pos) override;

		// move - move that brought us to this position.
		// pos - represents the current game.
		virtual void show(const Position & pos, const Move & move) override;

		// move - move that brought us to this position.
		// pos - represents the current game.
		virtual void show(const Position & pos, const MoveList & validMoves) override;

	private:

		guten::boards::CheckerBoard m_img;
	};
} // namespace forge