#include "TextView.h"

#include <Guten/draw/DrawFunctions.h>

using namespace std;

namespace forge
{
	TextView::TextView()
	{
		m_img.lightCell = guten::color::yellow;
		m_img.lightBoarder = guten::color::yellow;
		m_img.lightPiece = guten::color::brown;
	}

	void TextView::show(const Position& pos)
	{
		placePieces(pos.board());

		guten::core::Matrix theirInfo = placeInfo(pos, false);
		guten::core::Matrix ourInfo = placeInfo(pos, true);

		combineMats(theirInfo, ourInfo).print();
	}

	// move - move that brought us to this position.
	// pos - represents the current game.
	void TextView::show(const Position& pos, const Move& move)
	{
		placePieces(pos.board());

		m_img.highlight(guten::Point{ move.from().row(), move.from().col() });
		m_img.highlight(guten::Point{ move.to().row(), move.to().col() });

		guten::core::Matrix theirInfo = placeInfo(pos, forge::BLACK);
		guten::core::Matrix ourInfo = placeInfo(pos, forge::WHITE);

		combineMats(theirInfo, ourInfo).print();
	}

	void TextView::show(const Position& pos, const MoveList& validMoves)
	{
		placePieces(pos.board());

		// Highlight valid moves for some piece
		for (const MovePositionPair& pair : validMoves) {
			BoardSquare to = pair.move.to();

			m_img.highlight(to.row(), to.col(), guten::color::lightcyan, guten::color::cyan);
		}

		// Highlight moving piece
		if (validMoves.empty() == false) {
			BoardSquare from = validMoves.front().move.from();

			m_img.highlight(guten::Point{ from.row(), from.col() });
		}

		guten::core::Matrix theirInfo = placeInfo(pos, false);
		guten::core::Matrix ourInfo = placeInfo(pos, true);

		combineMats(theirInfo, ourInfo).print();
	}

	void TextView::highlightCells(
		BitBoard bb,
		const termcolor::Color& lightColor,
		const termcolor::Color& darkColor,
		guten::boards::CheckerBoard& board)
	{
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				if (bb[BoardSquare(row, col)] == 1) {
					board.highlight(row, col, lightColor, darkColor);
				}
			}
		}
	}

	void TextView::placePieces(const Board& board)
	{
		m_img = guten::boards::CheckerBoard{};

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				pieces::Piece p = board.at(row, col);

				m_img.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}
	}

	guten::core::Matrix TextView::placeInfo(const Position& pos, bool isWhite)
	{
		guten::core::Matrix info{ 10, 20 };

		const Board& b = pos.board();

		BitBoard pieces = (isWhite ? b.whites() : b.blacks());

		BitBoard Qs = b.queens() & pieces;
		BitBoard Bs = b.bishops() & pieces;
		BitBoard Ns = b.knights() & pieces;
		BitBoard Rs = b.rooks() & pieces;
		BitBoard Ps = b.pawns() & pieces;

		guten::draw::putText(info, (isWhite ? "White:" : "Black:"), guten::Point{ 0, 0 });
		guten::draw::putText(info, "Queens  = " + to_string(Qs.count()), guten::Point{ 1, 4 });
		guten::draw::putText(info, "Bishops = " + to_string(Bs.count()), guten::Point{ 2, 4 });
		guten::draw::putText(info, "Knights = " + to_string(Ns.count()), guten::Point{ 3, 4 });
		guten::draw::putText(info, "Rooks   = " + to_string(Rs.count()), guten::Point{ 4, 4 });
		guten::draw::putText(info, "Pawns   = " + to_string(Ps.count()), guten::Point{ 5, 4 });

		return info;
	}

	guten::core::Matrix TextView::combineMats(guten::core::Matrix& theirInfo, guten::core::Matrix& ourInfo)
	{
		/*
			+--------------------------------------------------------------------------------------------------------------+
			| +------------------------+-----------------------------------------+----------------------------------------+|
			| | Theirs:                |                                         |                                        ||
			| |    ...                 |                                         |                                        ||
			| |    ...                 |                                         |  Move History                          ||
			| +------------------------+            checker board                |                                        ||
			| +------------------------+                                         |                                        ||
			| | Ours:                  |                                         |                                        ||
			| |    ...                 |                                         |                                        ||
			| |    ...                 |                                         |                                        ||
			| +------------------------+-----------------------------------------+----------------------------------------+|
			+--------------------------------------------------------------------------------------------------------------+
		*/

		guten::core::Matrix cb = m_img.draw();

		guten::core::Matrix mat{
			cb.nRows() + 2,
			theirInfo.nCols() + cb.nCols() + 1
		};
		guten::draw::rectangle(mat, guten::Point{ 0, 0 }, mat.size(), guten::color::white, true);

		theirInfo.copyTo(mat, guten::Point{ 1, 1 });
		ourInfo.copyTo(mat, guten::Point{ theirInfo.nRows(), 1 });
		cb.copyTo(mat, guten::Point{ 1, theirInfo.nCols() });

		return mat;
	}
} // namespace forge