#include "Position.h"

using namespace std;

namespace forge
{
	template<> void Position::move<pieces::King>(Move move)
	{
#ifdef _DEBUG
		if (m_board.at(move.from()).isKing() == false) {
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
				<< ": This method only moves Kings\n";
		}
#endif // _DEBUG

		if (m_board.isWhite(move.from())) {
			this->move<pieces::WhiteKing>(move);
		}
		else {
			this->move<pieces::BlackKing>(move);
		}
	}

		template<> void Position::move<pieces::WhiteKing>(Move move) 
		{
	#ifdef _DEBUG
			if (m_board.at(move.from()).isKing() == false) {
				std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
					<< ": This method only moves Kings\n";
			}
	#endif // _DEBUG
	
			m_board.move<pieces::WhiteKing>(move);
	
			m_fiftyMoveRule.update();
			// TODO: Castling
	
			m_moveCounter++;
		}
	
		template<> void Position::move<pieces::BlackKing>(Move move) 
		{
	#ifdef _DEBUG
			if (m_board.at(move.from()).isKing() == false) {
				std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
					<< ": This method only moves Kings\n";
			}
	#endif // _DEBUG
	
			m_board.move<pieces::BlackKing>(move);
	
			m_fiftyMoveRule.update();
			// TODO: Castling
	
			m_moveCounter++;
		}
	
	///	template<> void Position::move<pieces::Queen>(Move move)
	///	{
	///		m_board.move<pieces::Queen>(move);
	///
	///		m_fiftyMoveRule.update();
	///
	///		m_moveCounter++;
	///	}
	///
	///	template<> void Position::move<pieces::Bishop>(Move move)
	///	{
	///		m_board.move<pieces::Bishop>(move);
	///
	///		m_fiftyMoveRule.update();
	///
	///		m_moveCounter++;
	///	}
	///
	///	template<> void Position::move<pieces::Knight>(Move move)
	///	{
	///		m_board.move<pieces::Knight>(move);
	///
	///		m_fiftyMoveRule.update();
	///
	///		m_moveCounter++;
	///	}
	///
	///	template<> void Position::move<pieces::QBN_Piece>(Move move)
	///	{
	///		m_board.move<pieces::QBN_Piece>(move);
	///
	///		m_fiftyMoveRule.update();
	///
	///		m_moveCounter++;
	///	}
	
	template<> void Position::move<pieces::Rook>(Move move)
	{
		m_board.move<pieces::Rook>(move);
	
		m_fiftyMoveRule.update();
	
		// TODO: Don't forget castling
	
		m_moveCounter++;
	}

	template<> void Position::move<pieces::WhitePawn>(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.whites()[move.from()] == false)
			std::cout << "Error " << __FILE__ << " line " << __LINE__
			<< ": This method only moves white pawns\n";
#endif

		// --- Was this a capture? ---
		if (m_board.isOccupied(move.to()))
			m_fiftyMoveRule.pieceCaptured();	// Yes. Capture occured

		m_board.place<pieces::Empty>(move.from(), bool());

		if (move.to().isTopRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.place<pieces::WhitePawn>(move.to(), true);

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();
		// TODO: ENPASSENT

		m_moveCounter++;
	}

	template<> void Position::move<pieces::BlackPawn>(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.blacks()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves black pawns\n";
#endif

		// --- Was this a capture? ---
		if (m_board.isOccupied(move.to()))
			m_fiftyMoveRule.pieceCaptured();	// Yes. Capture occured

		m_board.place<pieces::Empty>(move.from(), true);
		if (move.to().isBotRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.place<pieces::BlackPawn>(move.to());

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		// TODO: Still need enpassent 

		m_moveCounter++;
	}

	template<> void Position::move<pieces::Pawn>(Move move)
	{
		if (m_board.isWhite(move.from()))
			this->move<pieces::WhitePawn>(move);
		else
			this->move<pieces::BlackPawn>(move);
	}

	template<> void Position::move<pieces::Piece>(Move move)
	{
		pieces::Piece p = m_board.at(move.from());

		if (p.isPawn()) {
			if (p.isWhite()) {
				this->move<pieces::WhitePawn>(move);
			}
			else {
				this->move<pieces::BlackPawn>(move);
			}
		}
		else if (p.isQueen() || p.isBishop() || p.isKnight()) {
			this->move<pieces::QBN_Piece>(move);
		}
		else if (p.isRook()) {
			this->move<pieces::Rook>(move);
		}
		else if (p.isKing()) {
			this->move<pieces::King>(move);
		}
	}
} // namespace forge