#include "Position.h"

using namespace std;

namespace forge
{
	// ----- Push Moves -----

	template<> void Position::pushPiece<pieces::King>(Move move)
	{
		m_board.move<pieces::King>(move);

		m_fiftyMoveRule.update();
		// TODO: Castling

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::Queen>(Move move)
	{
		m_board.move<pieces::Queen>(move);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::Bishop>(Move move)
	{
		m_board.move<pieces::Bishop>(move);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::Knight>(Move move)
	{
		m_board.move<pieces::Knight>(move);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::Rook>(Move move)
	{
		m_board.move<pieces::Rook>(move);

		m_fiftyMoveRule.update();

		// TODO: Done forget castling

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::WhitePawn>(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.whites()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves white pawns\n";
		if (m_board.empty()[move.to()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		m_board.place<pieces::Empty>(move.from(), bool());

		if (move.to().isTopRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeWhitePawn(move.to());

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();
		// TODO: ENPASSENT

		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::BlackPawn>(Move move)
	{
///#ifdef _DEBUG 
///		if (m_board.pawns()[move.from()] == false || m_board.blacks()[move.from()] == false)
///			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
///			<< ": This method only moves black pawns\n";
///		if (m_board.empty()[move.to()] == false)
///			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
///			<< ": 'to' square must be empty when calling this method.\n";
///#endif
///
///		m_board.removePiece(move.from());
///		if (move.to().isBotRank()) m_board.placePiece(move.to(), move.promotion());
///		else m_board.placeBlackPawn(move.to());
///
///		m_fiftyMoveRule.pawnHasMoved();
///		m_fiftyMoveRule.update();
///
///		// TODO: Still need enpassent 
///
///		m_moveCounter++;
	}

	template<> void Position::pushPiece<pieces::Piece>(Move move)
	{
	///	m_board.movePiece(move.from(), move.to());
	///
	///	m_fiftyMoveRule.update();
	///
	///	m_moveCounter++;
	}

} // namespace forge