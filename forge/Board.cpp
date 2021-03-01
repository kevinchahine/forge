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
		return this->at(BoardSquare(
			static_cast<uint16_t>(row),
			static_cast<uint16_t>(col) ));
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

	void Board::placePiece(BoardSquare square, Piece piece)
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

	void Board::removePiece(BoardSquare pos)
	{
		m_whites.set(pos.val(), 0);
		m_blacks.set(pos.val(), 0);
		m_bishops.set(pos.val(), 0);
		m_rooks.set(pos.val(), 0);
		m_pawns.set(pos.val(), 0);
	}

	void Board::placeWhitePawn(BoardSquare pos)
	{
		m_whites.set(pos.val(), 1);
		m_pawns.set(pos.val(), 1);
	}

	void Board::placeBlackPawn(BoardSquare pos)
	{
		m_blacks.set(pos.val(), 1);
		m_pawns.set(pos.val(), 1);
	}

	void Board::placeWhiteRook(BoardSquare pos)
	{
		m_whites.set(pos.val(), 1);
		m_rooks.set(pos.val(), 1);
	}

	void Board::placeBlackRook(BoardSquare pos)
	{
		m_blacks.set(pos.val(), 1);
		m_rooks.set(pos.val(), 1);
	}

	void Board::placeWhiteKnight(BoardSquare pos)
	{
		m_whites.set(pos.val(), 1);
		// Thats it for knights
	}

	void Board::placeBlackKnight(BoardSquare pos)
	{
		m_blacks.set(pos.val(), 1);
		// Thats it for knights
	}

	void Board::placeWhiteBishop(BoardSquare pos)
	{
		m_whites.set(pos.val(), 1);
		m_bishops.set(pos.val(), 1);
	}

	void Board::placeBlackBishop(BoardSquare pos)
	{
		m_blacks.set(pos.val(), 1);
		m_bishops.set(pos.val(), 1);
	}

	void Board::placeWhiteQueen(BoardSquare pos)
	{
		m_whites.set(pos.val(), 1);
		m_rooks.set(pos.val(), 1);
		m_bishops.set(pos.val(), 1);
	}

	void Board::placeBlackQueen(BoardSquare pos)
	{
		m_blacks.set(pos.val(), 1);
		m_rooks.set(pos.val(), 1);
		m_bishops.set(pos.val(), 1);
	}

	void Board::moveWhiteKing(BoardSquare pos)
	{
		m_whites.set(m_whiteKing.val(), 0);	// "remove" white king
		m_whiteKing = pos;
		m_whites.set(m_whiteKing.val(), 1);	// "place" white king 
	}

	void Board::moveBlackKing(BoardSquare pos)
	{
		m_blacks.set(m_blackKing.val(), 0);	// "remove" black king
		m_blackKing = pos;
		m_blacks.set(m_blackKing.val(), 1);	// "place" black king 
	}

	void Board::reset()
	{
		(*this) = Board();
	}
} // namespace forge
