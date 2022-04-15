#include "Board.h"
#include "HashCombine.h"

#include <Guten/termcolor/termcolor.hpp>
#include <Guten/color/Color.h>
#include <Guten/boards/CheckerBoard.h>

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

		guten::boards::CheckerBoard cb = std::move(getCheckerBoard());
		cb.print();

		//os << guten::color::pop();
	}

	void Board::printMini(std::ostream & os) const
	{
		guten::core::Matrix miniBoard = getMiniBoard();

		miniBoard.print(0, os);
	}

	guten::boards::CheckerBoard Board::getCheckerBoard() const
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
		const termcolor::Color & lightPiece, 
		const termcolor::Color & darkPiece, 
		const termcolor::Color & lightCell, 
		const termcolor::Color & darkCell) const
	{
		guten::core::Matrix miniBoard;
		miniBoard.resize(8, 8);

		for (int row = 0; row < this->rows(); row++) {
			for (int col = 0; col < this->cols(); col++) {
				const pieces::Piece & p = (*this).at(row, col);

				termcolor::ColorFBG color;
				color.background = (row % 2 == col % 2 ? guten::color::yellow : guten::color::green);
				color.foreground = (p.isWhite() ? guten::color::white : guten::color::black);

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

		if (isEmpty(square)) {
			piece = pieces::empty;
		}
		else {
			// Just assume piece is white for now.
			// Well make sure at the bottom.

			if (isPawn(square)) { piece = pieces::whitePawn; }
			else if (isBishop(square)) { piece = pieces::whiteBishop; }
			else if (isRook(square)) { piece = pieces::whiteRook; }
			else if (isQueen(square)) { piece = pieces::whiteQueen; }
			else if (isKnight(square)) { piece = pieces::whiteKnight; }
			else { piece = pieces::whiteKing; }

			// Was the piece infact white or black?
			if (isBlack(square)) {
				// It should be black
				piece.flipColor();	// Change color to black;
			}
		}

		return piece;
	}

	Board Board::rotated() const
	{
		Board b;

		b.m_whites = this->m_blacks;
		b.m_blacks = this->m_whites;
		b.m_whites = m_whites.rotated();
		b.m_blacks = m_blacks.rotated();
		b.m_bishops = m_bishops.rotated();
		b.m_rooks = m_rooks.rotated();
		b.m_pawns = m_pawns.rotated();
		b.m_whiteKing = m_whiteKing.rotated();
		b.m_blackKing = m_blackKing.rotated();
		
		return b;
	}

	// TODO: This method is placed above Board::move<>() methods because some of them call it.
	// Try to move this method near the other place<>() methods.
	template<> void Board::place<pieces::Empty>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = 0;	// Both are necessary
		m_blacks[square] = 0;	// Both are necessary
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 0;
	}

	// ---------------------- MOVE METHODS (both push moves and captures) -----

	template<> void Board::move<pieces::WhiteKing>(Move move)
	{
		m_whites[move.to()] = 1;
		m_blacks[move.to()] = 0;
		m_bishops[move.to()] = 0;
		m_rooks[move.to()] = 0;
		m_pawns[move.to()] = 0;
		m_whiteKing = move.to();

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::BlackKing>(Move move)
	{
		m_whites[move.to()] = 0;
		m_blacks[move.to()] = 1;
		m_bishops[move.to()] = 0;
		m_rooks[move.to()] = 0;
		m_pawns[move.to()] = 0;
		m_blackKing = move.to();

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::King>(Move move)
	{
#ifdef _DEBUG
		if (!isKing(move.from())) {
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
				<< ": 'from' square must be a King when calling this method.\n";
		}
#endif // _DEBUG

		// Remove anything that might be on destination square
		place<pieces::Empty>(move.to(), true);

		// Figure out which King we are moving then move it?
		if (this->isWhite(move.from())) {
			m_whiteKing = move.to();
			m_whites[move.to()] = 1;
		}
		else {
			m_blackKing = move.to();
			m_blacks[move.to()] = 1;
		}

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::QBN_Piece>(Move move)
	{
		m_whites[move.to()] = m_whites[move.from()];
		m_blacks[move.to()] = m_blacks[move.from()];
		m_bishops[move.to()] = m_bishops[move.from()];
		m_rooks[move.to()] = m_rooks[move.from()];
		m_pawns[move.to()] = m_pawns[move.from()];

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::Queen>(Move move)
	{
		this->move<pieces::QBN_Piece>(move);
	}

	template<> void Board::move<pieces::Bishop>(Move move)
	{
		this->move<pieces::QBN_Piece>(move);
	}

	template<> void Board::move<pieces::Knight>(Move move)
	{
		this->move<pieces::QBN_Piece>(move);
	}

	template<> void Board::move<pieces::Rook>(Move move)
	{
		this->move<pieces::QBN_Piece>(move);

		// TODO: Castling: Dont forget castling rights
	}

	template<> void Board::move<pieces::WhitePawn>(Move move)
	{
		m_whites[move.to()] = 1;
		m_blacks[move.to()] = 0;
		m_bishops[move.to()] = 0;
		m_rooks[move.to()] = 0;
		m_pawns[move.to()] = 1;

		// TODO: Enpassent

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::BlackPawn>(Move move)
	{
		m_whites[move.to()] = 0;
		m_blacks[move.to()] = 1;
		m_bishops[move.to()] = 0;
		m_rooks[move.to()] = 0;
		m_pawns[move.to()] = 1;

		// TODO: Enpassent

		place<pieces::Empty>(move.from(), true);
	}

	template<> void Board::move<pieces::Pawn>(Move move)
	{
		if (isWhite(move.from())) {
			// === White Pawn ===
			this->move<pieces::WhitePawn>(move);
		}
		else {
			// === Black Pawn ===
			this->move<pieces::BlackPawn>(move);
		}
	}

	template<> void Board::move<pieces::Piece>(Move move)
	{
		pieces::Piece p = this->at(move.from());

		if (p.isKing()) {
			if (p.isWhite())
				this->move<pieces::WhiteKing>(move);
			else
				this->move<pieces::BlackKing>(move);
		}
		else if (p.isQueen() || p.isBishop() || p.isKnight()) {
			this->move<pieces::QBN_Piece>(move);
		}
		else if (p.isRook()) {
			this->move<pieces::Rook>(move);
		}
		else if (p.isPawn()) {
			if (p.isWhite())
				this->move<pieces::WhitePawn>(move);
			else
				this->move<pieces::BlackPawn>(move);
		}
#ifdef _DEBUG
		else {
			std::cout << "Error: " << __FILE__ << " line " << __LINE__
				<< " Piece = " << p << " move = " << move << std::endl;
		}
#endif // _DEBUG
	}

	// -------------------------------- PLACE ---------------------------------

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

	template<> void Board::place<pieces::WhiteKing>(BoardSquare square, bool isWhite)
	{
		if (square != m_whiteKing)
			move<pieces::WhiteKing>(Move{ m_whiteKing, square });
	}

	template<> void Board::place<pieces::BlackKing>(BoardSquare square, bool isWhite)
	{
		if (square != m_blackKing)
			move<pieces::BlackKing>(Move{ m_blackKing, square });
	}

	template<> void Board::place<pieces::King>(BoardSquare square, bool isWhite)
	{
		if (isWhite) {
			place<pieces::WhiteKing>(square, true);
		}
		else {
			place<pieces::BlackKing>(square, false);
		}
	}

	template<> void Board::place<pieces::Queen>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 1;
		m_rooks[square] = 1;
		m_pawns[square] = 0;
	}

	template<> void Board::place<pieces::Bishop>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 1;
		//m_rooks[square] = 0;
		//m_pawns[square] = 0;
	}

	template<> void Board::place<pieces::Knight>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 0;
	}

	template<> void Board::place<pieces::Rook>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 0;
		m_rooks[square] = 1;
		m_pawns[square] = 0;
	}

	template<> void Board::place<pieces::Pawn>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 1;
	}

	template<> void Board::place<pieces::WhitePawn>(BoardSquare square, bool isWhite)
	{
		// isWhite is irrelevent
		m_whites[square] = 1;
		m_blacks[square] = 0;
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 1;
	}

	template<> void Board::place<pieces::BlackPawn>(BoardSquare square, bool isWhite)
	{
		// isWhite is irrelevent
		m_whites[square] = 0;
		m_blacks[square] = 1;
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 1;
	}

	template<> BitBoard Board::pieces<pieces::King>() const { return kings(); }
	template<> BitBoard Board::pieces<pieces::WhiteKing>() const { return kings() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackKing>() const { return kings() & blacks(); }
	template<> BitBoard Board::pieces<pieces::Queen>() const { return queens(); }
	template<> BitBoard Board::pieces<pieces::WhiteQueen>() const { return queens() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackQueen>() const { return queens() & blacks(); }
	template<> BitBoard Board::pieces<pieces::Bishop>() const { return bishops(); }
	template<> BitBoard Board::pieces<pieces::WhiteBishop>() const { return bishops() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackBishop>() const { return bishops() & blacks(); }
	template<> BitBoard Board::pieces<pieces::Knight>() const { return knights(); }
	template<> BitBoard Board::pieces<pieces::WhiteKnight>() const { return knights() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackKnight>() const { return knights() & blacks(); }
	template<> BitBoard Board::pieces<pieces::Rook>() const { return rooks(); }
	template<> BitBoard Board::pieces<pieces::WhiteRook>() const { return rooks() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackRook>() const { return rooks() & blacks(); }
	template<> BitBoard Board::pieces<pieces::Pawn>() const { return pawns(); }
	template<> BitBoard Board::pieces<pieces::WhitePawn>() const { return pawns() & whites(); }
	template<> BitBoard Board::pieces<pieces::BlackPawn>() const { return pawns() & blacks(); }

	template<> bool Board::isPiece<pieces::King>(BoardSquare square) const { return isKing(square); }
	template<> bool Board::isPiece<pieces::Queen>(BoardSquare square) const { return isQueen(square); }
	template<> bool Board::isPiece<pieces::Bishop>(BoardSquare square) const { return isBishop(square); }
	template<> bool Board::isPiece<pieces::Knight>(BoardSquare square) const { return isKnight(square); }
	template<> bool Board::isPiece<pieces::Rook>(BoardSquare square) const { return isRook(square); }
	template<> bool Board::isPiece<pieces::WhitePawn>(BoardSquare square) const { return isWhite(square) && isPawn(square); }
	template<> bool Board::isPiece<pieces::BlackPawn>(BoardSquare square) const { return isBlack(square) && isPawn(square); }
	template<> bool Board::isPiece<colors::White>(BoardSquare square) const { return isWhite(square); }
	template<> bool Board::isPiece<colors::Black>(BoardSquare square) const { return isBlack(square); }

	template<> BitBoard Board::directionals<directions::Linear>() const { return laterals() & diagonals(); }
	template<> BitBoard Board::directionals<directions::Lateral>() const { return laterals(); }
	template<> BitBoard Board::directionals<directions::Diagonal>() const { return diagonals(); }
	template<> BitBoard Board::directionals<directions::LShape>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight0>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight1>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight2>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight3>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight4>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight5>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight6>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Knight7>() const { return knights(); }
	template<> BitBoard Board::directionals<directions::Ray>() const { return rays(); }
	template<> BitBoard Board::directionals<directions::Up>() const { return laterals(); }
	template<> BitBoard Board::directionals<directions::Down>() const { return laterals(); }
	template<> BitBoard Board::directionals<directions::Left>() const { return laterals(); }
	template<> BitBoard Board::directionals<directions::Right>() const { return laterals(); }
	template<> BitBoard Board::directionals<directions::UR>() const { return diagonals(); }
	template<> BitBoard Board::directionals<directions::UL>() const { return diagonals(); }
	template<> BitBoard Board::directionals<directions::DL>() const { return diagonals(); }
	template<> BitBoard Board::directionals<directions::DR>() const { return diagonals(); }

	template<> BitBoard Board::colors<colors::White>() const { return whites(); }
	template<> BitBoard Board::colors<colors::Black>() const { return blacks(); }

} // namespace forge

namespace std
{
	std::size_t hash<forge::Board>::operator()(const forge::Board& b) const noexcept
	{
		std::size_t seed = 0;

		hash_combine(seed, b.m_whites);
		hash_combine(seed, b.m_whites);
		hash_combine(seed, b.m_blacks);
		hash_combine(seed, b.m_bishops);
		hash_combine(seed, b.m_rooks);
		hash_combine(seed, b.m_pawns);
		hash_combine(seed, b.m_whiteKing);
		hash_combine(seed, b.m_blackKing);

		return seed;
	}
} // namespace std