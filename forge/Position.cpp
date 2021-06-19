#include "Position.h"

using namespace std;

namespace forge
{
	void Position::reset()
	{
		m_board.placeAllPieces();

		m_fiftyMoveRule.reset();

		m_moveCounter.reset();
	}

	void Position::clear()
	{
		m_board.reset();
		
		m_fiftyMoveRule.reset();

		m_moveCounter.reset();
	}


	void Position::moveWhitePawn(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.whites()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves white pawns\n";
		if (m_board.empty()[move.to()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		if (m_board.isOccupied(move.to()))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_board.removePiece(move.from());
		if (move.to().isTopRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeWhitePawn(move.to());

		// TODO: ENPASSENT

		m_moveCounter++;
	}

	void Position::moveWhitePawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG
		if (to.isTopRank()) {
			cout << "Error: " << __FUNCTION__ << " don't use this method for promotions\n";
		}
#endif // _DEBUG

		moveWhitePawn(Move{ from, to });
	}

	void Position::moveBlackPawn(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.blacks()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves black pawns\n";
		if (m_board.empty()[move.to()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		if (m_board.isOccupied(move.to()))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_board.removePiece(move.from());
		if (move.to().isBotRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeBlackPawn(move.to());

		// TODO: Still need enpassent 

		m_moveCounter++;
	}

	void Position::moveBlackPawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG
		if (to.isTopRank()) {
			cout << "Error: " << __FUNCTION__ << " don't use this method for promotions\n";
		}
#endif // _DEBUG

		moveBlackPawn(Move{ from, to });
	}

	void Position::moveRook(BoardSquare from, BoardSquare to)
	{
		if (m_board.isOccupied(to))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_board.movePiece(from, to);

		// TODO: Don't forget Castling

		m_moveCounter++;
	}

	void Position::moveRook(Move move)
	{
		moveRook(move.from(), move.to());
	}

	void Position::moveQBNR(BoardSquare from, BoardSquare to)
	{
		if (m_board.isOccupied(to))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_board.movePiece(from, to);

		m_moveCounter++;
	}

	void Position::moveQBNR(Move move)
	{
		moveQBNR(move.from(), move.to());
	}

	void Position::moveWhiteKing(BoardSquare to)
	{
		if (m_board.isOccupied(to))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_board.moveWhiteKing(to);

		// TODO: Don't forget castling

		m_moveCounter++;
	}

	void Position::moveBlackKing(BoardSquare to)
	{
		if (m_board.isOccupied(to))
			m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_board.moveBlackKing(to);

		// TODO: Don't forget castling

		m_moveCounter++;
	}
} // namespace forge