#pragma once

#include "forge/views/ViewBase.h"
#include "forge/movegen/MoveList.h"

#include "Guten/boards/CheckerBoard.h"

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
			const termcolor::Color & lightColor,
			const termcolor::Color & darkColor,
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