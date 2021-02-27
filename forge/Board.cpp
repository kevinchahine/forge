#include "Board.h"

#include <Guten/iocolor.h>
#include <Guten/Color.h>
#include <Guten/CheckerBoard.h>

using namespace std;

namespace forge
{
	void Board::print(std::ostream & os) const
	{
		os << guten::color::push();

		guten::boards::CheckerBoard disp;
		disp.drawBackground();

		for (int row = 0; row < this->rows(); row++) {
			for (int col = 0; col < this->cols(); col++) {
				const Piece & p = (*this).at(row, col);
				
				disp.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		disp.print(0, os);

		os << guten::color::pop();
	}

	Piece Board::at(int row, int col) const
	{
		return this->at(BoardSquare{
			static_cast<uint8_t>(row),
			static_cast<uint8_t>(col) });
	}

	Piece Board::at(BoardSquare square) const
	{
		Piece piece;

		if (~occupied()[square]) {
			piece = pieces::empty;
		}
		else {
			// Just assume piece is white for now.
			// Well make sure at the bottom.

			if (pawns()[square]) { piece = pieces::whitePawn; }
			else if (bishops()[square]) { piece = pieces::whiteBishop; }
			else if (rooks()[square]) { piece = pieces::whiteRook; }
			else if (queens()[square]) { piece = pieces::whiteQueen; }
			else if (knights()[square]) { piece = pieces::whiteKnight; }
			else { piece = pieces::whiteKing; }

			// Was the piece infact white or black?
			if (blacks()[square]) {
				// It should be black
				piece.flip();	// Change color to black;
			}
		}

		return piece;
	}

	void Board::setPiece(BoardSquare square, Piece piece)
	{
		if (piece.isEmpty()) {
			m_whites[square] = 0;
			m_blacks[square] = 0;
			m_bishops[square] = 0;
			m_rooks[square] = 0;
			m_pawns[square] = 0;

			// Do not worry about removing the King, we can't do that anyway
		}
		else {
			if (piece.isWhite()) {
				m_whites[square] = 1;
				m_blacks[square] = 0;
			}
			else /*if Black */ { 
				m_whites[square] = 0;
				m_blacks[square] = 1;
			}

			if (piece.isPawn()) {
				m_bishops[square] = 0;
				m_rooks[square] = 0;
				m_pawns[square] = 1;
			}
			else if (piece.isBishop()) {
				m_bishops[square] = 1;
				m_rooks[square] = 0;
				m_pawns[square] = 0;
			}
			else if (piece.isRook()) {
				m_bishops[square] = 0;
				m_rooks[square] = 1;
				m_pawns[square] = 0;
			}
			else if (piece.isQueen()) {
				m_bishops[square] = 1;
				m_rooks[square] = 1;
				m_pawns[square] = 0;
			}
			else if (piece.isKnight()) {
				m_bishops[square] = 0;
				m_rooks[square] = 0;
				m_pawns[square] = 0;
			}
			else if (piece.isKing()) {
				if (piece.isWhite()) {
					m_whiteKing = square;
				}
				else {
					m_blackKing = square;
				}
			}
		}
	}
} // namespace forge
