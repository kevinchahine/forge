#pragma once

#include <type_traits>

#include "HashCombine.h"
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
		friend struct std::hash<Position>;

		void reset();

		// Removes allToFen pieces
		void clear();

		// ----- Moves (both push moves and captures) -----
		// primary specialization works for all Pieces but Kings, Rooks and Pawns
		template <typename PIECE_T> void move(Move move);
		//////////////template<> void move<pieces::WhiteKing>(Move move);
		//////////////template<> void move<pieces::BlackKing>(Move move);
		//////////////template<> void move<pieces::King>(Move move);
		//////////////template<> void move<pieces::Queen>(Move move);
		//////////////template<> void move<pieces::Bishop>(Move move);
		//////////////template<> void move<pieces::Knight>(Move move);
		//////////////template<> void move<pieces::QBN_Piece>(Move move);
		//////////////template<> void move<pieces::Rook>(Move move);

		// Intended to be used from class MoveGenerator to move pieces
		// efficiently. 
		// Automatically applies "50 move rule" and increments move counter.
		// !!! Warning: This method is intended to be more efficient than robust.
		// Calling incorrectly can cause errors. Follow these rules when calling.
		//	- 'from' should point to piece of player whos turn it is.
		// Works with captures, promotions, castling and enpassent
		///////////////////template<> void move<pieces::WhitePawn>(Move move);
		///////////////////template<> void move<pieces::BlackPawn>(Move move);
		///////////////////template<> void move<pieces::Pawn>(Move move);
		///////////////////template<> void move<pieces::Piece>(Move move);

		// TODO: Add capture() and push() versions of move() that will be more efficient

		// --- NOTATIONS ---
		void fromFEN(const std::string& fen);
		std::string toFEN() const;

		Board & board() { return m_board; }
		const Board & board() const { return m_board; }
		const FiftyMoveRule & fiftyMoveRule() const { return m_fiftyMoveRule; }
		const MoveCounter & moveCounter() const { return m_moveCounter; }

		std::size_t hash() const noexcept 
		{
			std::size_t seed = 0;

			hash_combine(seed, this->board());
			hash_combine(seed, this->m_fiftyMoveRule);
			hash_combine(seed, this->m_moveCounter);

			return seed;
		}

		// Only compares board and current payers turn.
		bool operator==(const Position & rhs) const
		{
			return
				(m_moveCounter.isWhitesTurn() == rhs.m_moveCounter.isWhitesTurn()) &&
				(m_board == rhs.m_board);
		}
		bool operator!=(const Position & rhs) const { return !(*this == rhs); }

		bool operator<(const Position& rhs) const noexcept
		{
			return 
				this->hash() < 
				rhs.hash();
		}

		// Streams Position in FEN notation
		friend std::ostream& operator<<(std::ostream& os, const Position& pos);
		friend std::istream& operator>>(std::istream& is, Position& pos);

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

// --- Inject hash into std namespace
namespace std
{
	template<> struct hash<forge::Position>
	{
	public:
		std::size_t operator()(const forge::Position& pos) const noexcept
		{
			std::size_t seed = 0;

			hash_combine(seed, pos.board());
			hash_combine(seed, pos.m_fiftyMoveRule);
			hash_combine(seed, pos.m_moveCounter);

			return seed;
		}
	};
}