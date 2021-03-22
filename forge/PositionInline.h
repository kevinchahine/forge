#include "Position.h"

namespace forge
{
	inline void Position::moveWhitePawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[from] == false || m_board.whites()[from] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves white pawns\n";
		if (m_board.empty()[to] == false) std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif
		m_board.removePiece(from);
		m_board.placeWhitePawn(to);

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::moveBlackPawn(BoardSquare from, BoardSquare to)
	{
#ifdef _DEBUG 
		if (m_board.pawns()[from] == false || m_board.blacks()[from] == false)
			std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": This method only moves black pawns\n";
		if (m_board.empty()[to] == false) std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
			<< ": 'to' square must be empty when calling this method.\n";
#endif

		m_board.removePiece(from);
		m_board.placeBlackPawn(to);

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::whitePawnCapture(BoardSquare from, BoardSquare to)
	{
		// 1.) Capture piece first 
		m_board.removePiece(to);

		// 2.) Move Pawn
		m_board.removePiece(from);
		m_board.placeWhitePawn(to);

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::blackPawnCapture(BoardSquare from, BoardSquare to)
	{
		// 1.) Capture piece first 
		m_board.removePiece(to);

		// 2.) Move Pawn
		m_board.removePiece(from);
		m_board.placeBlackPawn(to);

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::enPassentWhitePawn(BoardSquare from, BoardSquare to)
	{
		// 1.) Capture piece first 
		m_board.removePiece(to.downOne());

		// 2.) Move Pawn
		m_board.removePiece(from);
		m_board.placeWhitePawn(to);

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::enPassentBlackPawn(BoardSquare from, BoardSquare to)
	{
		// 1.) Capture piece first 
		m_board.removePiece(to.upOne());

		// 2.) Move Pawn
		m_board.removePiece(from);
		m_board.placeBlackPawn(to);

		// 3.) Update rules
		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::promoteWhitePawn(BoardSquare from, BoardSquare to, Piece promotion)
	{
		// 1.) Remove piece incase a capture was made
		m_board.removePiece(to);

		m_board.removePiece(from);
		m_board.placePiece(to, promotion);

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::promoteBlackPawn(BoardSquare from, BoardSquare to, Piece promotion)
	{
		// 1.) Remove piece incase a capture was made
		m_board.removePiece(to);

		m_board.removePiece(from);
		m_board.placePiece(to, promotion);

		m_fiftyMoveRule.pawnHasMoved();
		m_fiftyMoveRule.update();

		m_moveCounter++;
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
	
	inline void Position::whiteKingCapture(BoardSquare to)
	{
		m_board.removePiece(to);
		m_board.moveWhiteKing(to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}

	inline void Position::blackKingCapture(BoardSquare to)
	{
		m_board.removePiece(to);
		m_board.moveBlackKing(to);

		m_fiftyMoveRule.pieceCaptured();
		m_fiftyMoveRule.update();

		m_moveCounter++;
	}
} // namespace forge