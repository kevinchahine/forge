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

	void Board::printMini(std::ostream & os) const
	{
		os << guten::color::push();
		
		for (int row = 0; row < this->rows(); row++) {
			for (int col = 0; col < this->cols(); col++) {
				const Piece & p = (*this).at(row, col);

				guten::color::Color color;
				color.setbg(row % 2 == col % 2 ? guten::color::brown : guten::color::green);
				color.setfg(p.isWhite() ? guten::color::yellow : guten::color::black);

				os << color << p;
			}
			os << '\n';
		}
		os << '\n';

		os << guten::color::pop();
	}

	Piece Board::at(int row, int col) const
	{
		return this->at(BoardSquare(
			static_cast<uint16_t>(row),
			static_cast<uint16_t>(col)));
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

	void Board::removePiece(BoardSquare pos)
	{
		m_whites.set(pos.val(), 0);
		m_blacks.set(pos.val(), 0);
		m_bishops.set(pos.val(), 0);
		m_rooks.set(pos.val(), 0);
		m_pawns.set(pos.val(), 0);
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
		// Which king are we moving?
		if (m_whiteKing == from) {
			// White king moves
			m_whiteKing = to;
		}
		else {
			// Black king moves
			m_blackKing = to;
		}
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

		m_whiteKing = BoardSquare{ 0, 4 };
		m_blackKing = BoardSquare{ 7, 4 };
		
		return;
	}

	MoveList Board::generatePsuedoLegalMovesWhite() const
	{
		MoveList moves;

		moves.reserve(35);	// At any point in a chess game there should be about 35 - 38 legal moves

		BitBoard isOccupied = occupied();
		BitBoard isEmpty = empty();

		// traverse each cell and look for white pieces
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				// Does this cell contain a white piece?
				BoardSquare pos(row, col);

				if (m_whites[pos] == 1) {
					// Yes this is a white piece.
					if (isPawn(pos)) {
						// Can pawn move 1
						if (pos.row() == 1) {
							// Promotion time. !!! TODO: Do this promotion

						}
						else if (isEmpty[pos.upOne()]) {
							moves.emplace_back(Move{ pos, pos.upOne() }, *this);
							moves.back().second.removePiece(pos);
							moves.back().second.placeWhitePawn(pos.upOne());

							// Can it move a second time
							if (pos.row() == 6 && isEmpty[pos.up(2)]) {
								moves.emplace_back(Move{ pos, pos.up(2) }, *this);
								moves.back().second.removePiece(pos);
								moves.back().second.placeWhitePawn(pos.up(2));
							}
						}
						
						// TODO: Still Need captures
						// TODO: Still need enpassent
					}
					else if (isRook(pos)) {
						BoardSquare p = pos;

						// --- Ups ---
						// traverse upward. Make sure we don't go past the top boarder
						while (p.row() != 0) {
							p = p.upOne();

							// --- Moves ---
							// Can Rook move up?
							if (isEmpty[p]) {
								moves.emplace_back(Move{ pos, p }, *this);
								moves.back().second.removePiece(pos);
								moves.back().second.placeWhiteRook(p);
							}
							// If rook moves up will it capture a Black piece?
							else if (m_blacks[p]) {
								moves.emplace_back(Move{ pos, p }, *this);
								moves.back().second.removePiece(pos);	// Pick up White Rook
								moves.back().second.removePiece(p);		// Remove captured Black Piece
								moves.back().second.placeWhiteRook(p);	// Place White Rook in its place
							}
							else { /* Must be a White piece */
								break; // A piece can't jump/capture it's own pieces.
							}
						}

						// --- Downs ---

						// --- Lefts ---

						// --- Rights ---

					}
					else if (isKnight(pos)) {
					}
					else if (isBishop(pos)) {
					}
					else if (isQueen(pos)) {
					}
					else if (isKing(pos)) {
					}
					else {
						// It must be an error.
						// May cause problems with UCI
						cout << "Uhhoo" << __FUNCTION__ << " line " << __LINE__ << '\n';
					}
				} // if (m_whites
			} // for (col
		} // for (row

		moves.shrink_to_fit();
		return  moves;
	}

	MoveList Board::generatePsuedoLegalMovesBlack() const
	{
		MoveList moves;

		moves.reserve(35);	// At any point in a chess game there should be about 35 - 38 legal moves

		BitBoard isOccupied = occupied();
		BitBoard isEmpty = empty();

		// traverse each cell and look for white pieces
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				// Does this cell contain a white piece?
				BoardSquare pos(row, col);

				if (m_blacks[pos] == 1) {
					// Yes this is a black piece.
					if (isPawn(pos)) {
						// Can pawn move 1
						if (pos.row() == 1) {
							// Promotion time. !!! TODO: Do this promotion

						}
						else if (isEmpty[pos.downOne()]) {
							moves.emplace_back(Move{ pos, pos.downOne() }, *this);
							moves.back().second.removePiece(pos);
							moves.back().second.placeBlackPawn(pos.downOne());

							// Can it move a second time
							if (pos.row() == 6 && isEmpty[pos.down(2)]) {
								moves.emplace_back(Move{ pos, pos.down(2) }, *this);
								moves.back().second.removePiece(pos);
								moves.back().second.placeBlackPawn(pos.down(2));
							}
						}

						// TODO: Still Need captures
						// TODO: Still need enpassent
					}
					else if (isRook(pos)) {
						BoardSquare p = pos;

						// --- Ups ---
						// traverse upward. Make sure we don't go past the top boarder
						while (p.row() != 0) {
							p = p.upOne();

							// --- Moves ---
							// Can Rook move up?
							if (isEmpty[p]) {
								moves.emplace_back(Move{ pos, p }, *this);
								moves.back().second.removePiece(pos);
								moves.back().second.placeBlackRook(p);
							}
							// If rook moves up will it capture a White piece?
							else if (m_whites[p]) {
								moves.emplace_back(Move{ pos, p }, *this);
								moves.back().second.removePiece(pos);	// Pick up Black Rook
								moves.back().second.removePiece(p);		// Remove captured White Piece
								moves.back().second.placeBlackRook(p);	// Place Black Rook in its place
							}
							else { /* Must be a Black piece */
								break; // A piece can't jump/capture it's own pieces.
							}
						}

						// --- Downs ---

						// --- Lefts ---

						// --- Rights ---

					}
					else if (isKnight(pos)) {
					}
					else if (isBishop(pos)) {
					}
					else if (isQueen(pos)) {
					}
					else if (isKing(pos)) {
					}
					else {
						// It must be an error.
						// May cause problems with UCI
						cout << "Uhhoo" << __FUNCTION__ << " line " << __LINE__ << '\n';
					}
				} // if (m_whites
			} // for (col
		} // for (row

		moves.shrink_to_fit();
		return  moves;
	}
} // namespace forge
