#pragma once

#include "Board.h"

namespace forge
{
	// -------------------------------- PLACE ---------------------------------

	template<> void Board::place<pieces::Empty>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = 0;	// Both are necessary
		m_blacks[square] = 0;	// Both are necessary
		m_bishops[square] = 0;
		m_rooks[square] = 0;
		m_pawns[square] = 0;
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

	// ---------------------- MOVE METHODS (both push moves and captures) -----

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

		// TODO: Dont forget castling rights
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
} // namespace forge