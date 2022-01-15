#pragma once

#include "ViewBase.h"

#include "MoveGenerator.h"
#include "Guten/boards/CheckerBoard.h"
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

		static void highlightCells(
			BitBoard bb, 
			const guten::color::Color & lightColor,
			const guten::color::Color & darkColor,
			guten::boards::CheckerBoard & board);

	protected:
		void placePieces(const Board & board);

		guten::core::Matrix placeInfo(const Position& pos, bool isWhite);

		void placeMoveHistory(const Position& pos);

		guten::core::Matrix combineMats(guten::core::Matrix& theirInfo, guten::core::Matrix& ourInfo);

	private:

		guten::boards::CheckerBoard m_img;
	};
} // namespace forge