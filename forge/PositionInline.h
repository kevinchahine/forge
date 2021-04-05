#include "Position.h"

using namespace std;

namespace forge
{
	inline void Position::moveWhitePawn(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.whites()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves white pawns\n";
		if (m_board.empty()[move.to()] == false) 
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		m_board.removePiece(move.from());
		if (move.to().isTopRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeWhitePawn(move.to());

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();
		// TODO: ENPASSENT

		m_moveCounter++;
	}

	inline void Position::moveWhitePawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG
		if (to.isTopRank()) {
			cout << "Error: " << __FUNCTION__ << " don't use this method for promotions\n";
		}
#endif // _DEBUG

		moveWhitePawn(Move{ from, to });
	}

	inline void Position::moveBlackPawn(Move move)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[move.from()] == false || m_board.blacks()[move.from()] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves black pawns\n";
		if (m_board.empty()[move.to()] == false) 
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif

		m_board.removePiece(move.from());
		if (move.to().isBotRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeBlackPawn(move.to());

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		// TODO: Still need enpassent 

		m_moveCounter++;
	}

	inline void Position::moveBlackPawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG
		if (to.isTopRank()) {
			cout << "Error: " << __FUNCTION__ << " don't use this method for promotions\n";
		}
#endif // _DEBUG

		moveBlackPawn(Move{ from, to });
	}

	inline void Position::captureWithWhitePawn(Move move)
	{
		// 1.) Capture piece first 
		m_board.removePiece(move.to());

		// 2.) Move Pawn
		m_board.removePiece(move.from());
		if (move.to().isTopRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeWhitePawn(move.to());

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::captureWithBlackPawn(Move move)
	{
		// 1.) Capture piece first 
		m_board.removePiece(move.to());

		// 2.) Move Pawn
		m_board.removePiece(move.from());
		if (move.to().isBotRank()) m_board.placePiece(move.to(), move.promotion());
		else m_board.placeBlackPawn(move.to());

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}
	
	inline void Position::moveRook(BoardSquare from, BoardSquare to)
	{
		// TODO: Don't forget Castling
		m_board.movePiece(from, to);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::moveRook(Move move)
	{
		moveRook(move.from(), move.to());
	}

	inline void Position::captureWithRook(BoardSquare from, BoardSquare to)
	{
		// TODO: Don't forget castling
		m_board.removePiece(to);		// TODO: Do we really need this?
		m_board.movePiece(from, to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::captureWithRook(Move move)
	{
		captureWithRook(move.from(), move.to());
	}

	inline void Position::moveQBNR(BoardSquare from, BoardSquare to)
	{
		m_board.movePiece(from, to);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::moveQBNR(Move move)
	{
		moveQBNR(move.from(), move.to());
	}

	inline void Position::captureWithQBNR(BoardSquare from, BoardSquare to)
	{
		m_board.removePiece(to);		// TODO: Do we really need this?
		m_board.movePiece(from, to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::captureWithQBNR(Move move)
	{
		captureWithQBNR(move.from(), move.to());
	}

	inline void Position::moveWhiteKing(BoardSquare to)
	{
		m_board.moveWhiteKing(to);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}
	
	inline void Position::moveBlackKing(BoardSquare to)
	{
		m_board.moveBlackKing(to);

		m_fiftyMoveRule.update();

		m_moveCounter++;
	}
	
	inline void Position::captureWithWhiteKing(BoardSquare to)
	{
		m_board.removePiece(to);
		m_board.moveWhiteKing(to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::captureWithBlackKing(BoardSquare to)
	{
		m_board.removePiece(to);
		m_board.moveBlackKing(to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}
} // namespace forge