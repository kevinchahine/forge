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
				const pieces::Piece & p = (*this).at(row, col);

				disp.placePiece(p.getCh(), row, col, p.isWhite());
			}
		}

		disp.print(0, os);

		os << guten::color::pop();
	}

	void Board::printMini(std::ostream & os) const
	{
		guten::core::Matrix miniBoard = getMiniBoard();

		miniBoard.print(0, os);
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

	void Board::removePiece(BoardSquare pos)
	{
		m_whites.set(pos.val(), 0);
		m_blacks.set(pos.val(), 0);
		m_bishops.set(pos.val(), 0);
		m_rooks.set(pos.val(), 0);
		m_pawns.set(pos.val(), 0);
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

	void Board::moveKing(BoardSquare pos, bool isWhite)
	{
		if (isWhite) { moveWhiteKing(pos); }
		else { moveBlackKing(pos); }
	}

	void Board::moveKing(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG 
		if (isKing(from) == false) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves kings\n";
		if (occupied()[to] == true) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		// Which king are we moving?
		if (m_whiteKing == from) {
			// We're moving the WHITE king.
			moveWhiteKing(to);
		}
		else {
			// We're moving the BLACK king.
			moveBlackKing(to);
		}
	}

	void Board::movePiece(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG
		if (isKing(from)) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method can't move kings\n";
		if (occupied()[to] == true) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif

		m_whites[to] = m_whites[from];
		m_blacks[to] = m_blacks[from];
		m_bishops[to] = m_bishops[from];	
		m_rooks[to] = m_rooks[from];
		m_pawns[to] = m_pawns[from];

		removePiece(from);
	}

	void Board::reset()
	{
		(*this) = Board();
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
