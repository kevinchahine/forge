#include "Position.h"

using namespace std;

namespace forge
{
	void Position::reset()
	{
		m_board.placeAllPieces();

		_50_move_rule = 0;

		m_moveCounter.reset();
	}
	
	void Position::applyMoveFast(Move move)
	{
		register BoardSquare from = move.from();
		register BoardSquare to = move.to();
		register Piece promotion = move.promotion();	// If promoting, what will we promot to. Might be empty.
		register Piece fromPiece = m_board.at(from);	// Piece we are moving from
		register Piece toPiece = m_board.at(to);		// Piece we are moving to

		// Look for captures
		if (toPiece.isEmpty() == false) {
			// A capture is being made.
			_50_move_rule = -1;	// reset 50 move rule
		}

		// Removing toPiece must be done before moving other pieces to this square
		// Remember that tecnically kings can't be captured
		// Assuming move is valid, we don't need to check if we are 
		// removing a King because that would be an invalid move.
		m_board.removePiece(to);

		if (fromPiece.isPawn()) {
			if (fromPiece.isWhite()) {
				// Is this a promotion?
				if (to.isTopRank()) {
					m_board.removePiece(from);
					m_board.placePiece(to, promotion);
				}
				// Is this en passent?
				else if (false) {
					// TODO: 
				}
			}
			else { // fromPiece.isBlack()
			// Is this a promotion?
				if (to.isBotRank()) {
					m_board.removePiece(from);
					m_board.placePiece(to, promotion);
				}
				// Is this en passent?
				else if (false) {
					// TODO: 
				}
			}
			// regular pawn move
			_50_move_rule = -1;	// reset 50 move rule
			m_board.removePiece(from);
			m_board.placePiece(to, fromPiece);
		}
		else if (fromPiece.isKing()) {
			// Is this a castling move?
			// Do we still have castling rights?
			// TODO: 

			// Moving Kings is done a little different than moving other pieces
			m_board.moveKing(from, to);
		}
		else {
			// Move other pieces
			m_board.removePiece(from);
			m_board.placePiece(to, fromPiece);
		}

		_50_move_rule++;
		++m_moveCounter;
	}

	bool Position::applyMoveSafe(Move move)
	{
		//cout << __FUNCTION__ << " Not implemented" << endl;
		applyMoveFast(move);	// TODO: for now

		//register BoardSquare from = move.from();
		//register BoardSquare to = move.to();

		//if (m_board.occupied()[from] == true) {
		//	if (m_moveCounter.isWhitesTurn()) {
		//		if (m_board.whites()[from] == false) {
		//			return false;	// White can't pick up a black piece
		//		}
		//	}
		//	else {
		//		if (m_board.blacks()[from] == false) {
		//			return false;	// black can't pick up a white piece
		//		}
		//	}

		//	// Now we know that from cooresponds to a piece of the color the the current player
		//	register Piece pickedUpPiece = m_board.at(from);

		//	// If its a King
		//	if (pickedUpPiece.isKing()) {
		//		m_board.moveKing(to, pickedUpPiece.isWhite());
		//	}
		//	if (pickedUpPiece.isPawn()) {
		//	}
		//	else {
		//		// Check for captures
		//		Piece toPiece = m_board.at(to);

		//		if (toPiece.isEmpty()) {
		//			// No capture
		//		}
		//	}
		//}
		//else {
		//	// You tried to pick up a piece that isn't there
		//	return false;	// Invalid Move
		//}

		//m_moveCounter++;

		return true;	// TODO: this is wrong
	}

	void Position::applyMove(const Position & position)
	{
		(*this) = position;
	}
} // namespace forge