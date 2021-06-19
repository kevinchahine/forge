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
	
	template<> void Board::place<pieces::Queen>(BoardSquare square, bool isWhite)
	{
		m_whites[square] = isWhite;
		m_blacks[square] = !isWhite;
		m_bishops[square] = 1;
		m_rooks[square] = 1;
		//m_pawns[square] = 0;
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
	
	}
	
	template<> void Board::place<pieces::Rook>(BoardSquare square, bool isWhite)
	{
	
	}
	
	template<> void Board::place<pieces::WhitePawn>(BoardSquare square, bool isWhite)
	{
	
	}
	
	template<> void Board::place<pieces::BlackPawn>(BoardSquare square, bool isWhite)
	{
	
	}

	template<> void Board::move<pieces::Empty>(Move move)
	{
	
	}
	
	template<> void Board::move<pieces::Queen>(Move move)
	{
#ifdef _DEBUG
		if (isKing(from)) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method can't move kings\n";
		if (occupied()[to] == true) cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif

		m_whites[move.to()] = m_whites[move.from()];
		m_blacks[move.to()] = m_blacks[move.from()];
		m_bishops[move.to()] = m_bishops[move.from()];
		m_rooks[move.to()] = m_rooks[move.from()];
		m_pawns[move.to()] = m_pawns[move.from()];

		place<pieces::Empty>(move.from(), true);
	}
	
	template<> void Board::move<pieces::Bishop>(Move move)
	{
		
	}
	
	template<> void Board::move<pieces::Knight>(Move move)
	{
	
	}
	
	template<> void Board::move<pieces::Rook>(Move move)
	{
	
	}
	
	template<> void Board::move<pieces::WhitePawn>(Move move)
	{
	
	}
	
	template<> void Board::move<pieces::BlackPawn>(Move move)
	{
	
	}
	
	template<> void Board::move<pieces::King>(Move move)
	{
	
	}
	

} // namespace forge