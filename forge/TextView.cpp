#include "TextView.h"

namespace forge
{
	void TextView::highlight(const BoardSquare & cell)
	{
		m_img.highlight(cell.row(), cell.col());
	}

	// move - move that brought us to this position.
	// pos - represents the current game.
	void TextView::show(const Move & move, const Position & pos)
	{
		const Board & b = pos.board();

		m_img.drawBackground();

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				Piece p = b.at(row, col);

				m_img.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		m_img.highlight(guten::Point{ move.from().row(), move.from().col() });
		m_img.highlight(guten::Point{ move.to().row(), move.to().col() });

		m_img.print();
	}
} // namespace forge