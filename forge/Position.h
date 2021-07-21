#pragma once

#include "Board.h"
#include "MoveCounter.h"
#include "FiftyMoveRule.h"

namespace forge
{
	class MoveGenerator;

	// Stores:
	//	- board (including pieces, castling rules, and enpassent)
	//	- 50 ply rule
	//	- move counter (tells whos turn it is)
	// Does not store:
	//	- time control
	//	- info about repetitions (Need game history for that)
	//	- game state: Win, Draw, Loose	(Need game history for that)
	class Position
	{
	public:
		// --- Declare Friend Classes ---
		friend MoveGenerator;

		void reset();

		// Removes all pieces
		void clear();

		// ----- Moves (both push moves and captures) -----
		// primary specialization works for all but Kings, Rooks and Pawns
		template <typename PIECE_T> void move(Move move) {
#ifndef _DEBUG
			if (m_board.at(move.from()).isKing() == false) {
				std::cout << "Error " << __FUNCTION__ << " line " << __LINE__
					<< ": This method only moves Kings\n";
			}
#endif // _DEBUG

			static_assert(
				(!std::is_base_of<PIECE_T, pieces::King>() &&
				!std::is_base_of<PIECE_T, pieces::Pawn>() &&
				!std::is_same<PIECE_T, pieces::Rook>()),
				"This specialization can't be called on Kings, Rooks or Pawns.");

			// --- Was this a capture? ---
			if (m_board.isOccupied(move.to()))
				m_fiftyMoveRule.pieceCaptured();	// Yes. Capture occured

			m_fiftyMoveRule.update();
			// TODO: Castling

			m_board.move<PIECE_T>(move);

			m_moveCounter++;
		}
		template<> void move<pieces::King>(Move move);
		template<> void move<pieces::WhiteKing>(Move move);
		template<> void move<pieces::BlackKing>(Move move);
		///template<> void move<pieces::Queen>(Move move);
		///template<> void move<pieces::Bishop>(Move move);
		///template<> void move<pieces::Knight>(Move move);
		///template<> void move<pieces::QBN_Piece>(Move move);
		template<> void move<pieces::Rook>(Move move);
		// Intended to be used from class MoveGenerator to move pieces
		// efficiently. 
		// Automatically applies "50 move rule" and increments move counter.
		// !!! Warning: This method is intended to be more efficient than robust.
		// Calling incorrectly can cause errors. Follow these rules when calling.
		//	- 'from' should point to piece of player whos turn it is.
		// Works with captures, promotions, castling and enpassent
		template<> void move<pieces::WhitePawn>(Move move);
		template<> void move<pieces::BlackPawn>(Move move);
		template<> void move<pieces::Pawn>(Move move);
		template<> void move<pieces::Piece>(Move move);

		// TODO: Add capture() and push() versions of move() that will be more efficient

		Board & board() { return m_board; }
		const Board & board() const { return m_board; }
		const FiftyMoveRule & fiftyMoveRule() const { return m_fiftyMoveRule; }
		const MoveCounter & moveCounter() const { return m_moveCounter; }

		// Only compares board and current payers turn.
		bool operator==(const Position & rhs) const
		{
			return
				(m_moveCounter.isWhitesTurn() == rhs.m_moveCounter.isWhitesTurn()) &&
				(m_board == rhs.m_board);
		}
		bool operator!=(const Position & rhs) const { return !(*this == rhs); }

	protected:
		Board m_board;

		// If in 50 moves no captures have been made and no pawns moved,
		// then its a draw.
		FiftyMoveRule m_fiftyMoveRule;

		// Number of moves played
		MoveCounter m_moveCounter;
	};
} // namespace forge

#include "PositionDefinitions.h"