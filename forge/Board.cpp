#include "Board.h"

#include <Guten/iocolor.h>
#include <Guten/Color.h>
#include <Guten/CheckerBoard.h>

using namespace std;

namespace forge
{
	void Board::reset()
	{
		(*this) = Board();
	}

	void Board::print(std::ostream & os) const
	{
		//os << guten::color::push();

		guten::boards::CheckerBoard cb = std::move(getImage());
		cb.print();

		//os << guten::color::pop();
	}

	void Board::printMini(std::ostream & os) const
	{
		guten::core::Matrix miniBoard = getMiniBoard();

		miniBoard.print(0, os);
	}

	guten::boards::CheckerBoard Board::getImage() const
	{
		guten::boards::CheckerBoard disp;
		disp.draw();

		for (int row = 0; row < this->rows(); row++) {
			for (int col = 0; col < this->cols(); col++) {
				const pieces::Piece & p = (*this).at(row, col);

				disp.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		return disp;
	}

	guten::core::Matrix Board::getMiniBoard(
		const guten::color::Color & lightPiece, 
		const guten::color::Color & darkPiece, 
		const guten::color::Color & lightCell, 
		const guten::color::Color & darkCell) const
	{
		guten::core::Matrix miniBoard;
		miniBoard.resize(8, 8);

		for (int row = 0; row < this->rows(); row++) {
			for (int col = 0; col < this->cols(); col++) {
				const pieces::Piece & p = (*this).at(row, col);

				guten::color::Color color;
				color.setbg(row % 2 == col % 2 ? guten::color::yellow : guten::color::green);
				color.setfg(p.isWhite() ? guten::color::white : guten::color::black);

				miniBoard.at(row, col).color = color;
				miniBoard.at(row, col).character = p.getCh();
			}
		}
		
		return miniBoard;
	}

	pieces::Piece Board::at(int row, int col) const
	{
		return this->at(BoardSquare(
			static_cast<uint16_t>(row),
			static_cast<uint16_t>(col)));
	}

	pieces::Piece Board::at(BoardSquare square) const
	{
		pieces::Piece piece;

		if (empty()[square]) {
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
				piece.flipColor();	// Change color to black;
			}
		}

		return piece;
	}

	void Board::placePiece(BoardSquare square, pieces::Piece piece)
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
	
	void Board::placePieces(BitBoard squares, pieces::Piece piece)
	{
		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				BoardSquare bs(row, col);
	
				if (squares[bs] == 1) {
					placePiece(bs, piece);
				}
			}
		}
	}

	void Board::placeAllPieces()
	{
		m_whites =	0b11111111'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
		m_blacks =	0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'11111111;
		m_bishops = 0b00101100'00000000'00000000'00000000'00000000'00000000'00000000'00101100;
		m_rooks =	0b10001001'00000000'00000000'00000000'00000000'00000000'00000000'10001001;
		m_pawns =	0b00000000'11111111'00000000'00000000'00000000'00000000'11111111'00000000;

		m_blackKing = BoardSquare{ 0, 4 };
		m_whiteKing = BoardSquare{ 7, 4 };
		
		return;
	}

} // namespace forge
