#include "TextView.h"

using namespace std;

namespace forge
{
	TextView::TextView()
	{
		m_img.lightCell = guten::color::yellow;
		m_img.lightBoarder = guten::color::yellow;

		m_img.lightPiece = guten::color::brown;
	}

	void TextView::show(const Position & pos)
	{
		const Board & b = pos.board();

		m_img.drawBackground();

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				Piece p = b.at(row, col);

				m_img.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		m_img.print();
	}

	// move - move that brought us to this position.
	// pos - represents the current game.
	void TextView::show(const Position & pos, const Move & move)
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

	void TextView::show(const Position & pos, const MoveList & validMoves)
	{
		const Board & b = pos.board();

		m_img.drawBackground();

		// Place pieces
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				Piece p = b.at(row, col);

				m_img.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		// Highlight valid moves for some piece
		for (const MovePositionPair & pair : validMoves) {
			BoardSquare to = pair.move.to();

			m_img.highlight(to.row(), to.col(), guten::color::lightcyan, guten::color::cyan);
		}

		// Highlight moving piece
		if (validMoves.empty() == false) {
			BoardSquare from = validMoves.front().move.from();

			m_img.highlight(guten::Point{ from.row(), from.col() });
		}

		m_img.print();
	}
} // namespace forge