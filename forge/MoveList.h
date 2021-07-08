#pragma once

#include "MovePositionPair.h"
#include "AttackChecker.h"

#include <vector>

namespace forge
{
#ifdef _DEBUG
	inline void printError(forge::Move m, const string & funcName) {
		std::cout << "Illegal move found :p, " << m << " in " << funcName << "\n";
	}
#else
#define printError(MOVE, FUNCNAME)	// empty placeholder
#endif

	// TODO: consider inheriting with protected instead of public to prevent 
	//		users from calling push_back and emplace back
	class MoveList : public std::vector<MovePositionPair>
	{
	public:
		MoveList() = default;
		MoveList(const MoveList &) = default;
		MoveList(MoveList &&) noexcept = default;
		virtual ~MoveList() noexcept = default;
		MoveList & operator=(const MoveList &) = default;
		MoveList & operator=(MoveList &&) noexcept = default;

		// Purpose: Pushes move to MoveListl
		// In Debug mode, it also verifies that Move was legal by seeing if the King
		// is being attacked. 
		template<typename PIECE_T>
		void emplace_back(Move move, const Position & currPos);

		// Appends 0 or more MovePositionPairs to end of container.
		// Iterates from begin (inclusive) to end (exclusive) in specified direction 
		// and appends MovePositionPairs put only if a 1 exists in moveMask.
		// PIECE_T - Piece type to be moved.
		// RAY_DIRECTION_T - Direction between begin and end. Must be a ray direction.
		// begin - Starting square (inclusive).
		// piece - Coordinate of piece that is moving 
		// end - Ending square (exclusive).
		// moveMask - BitBoard containing 1s where ever the piece may move to (push/captures).
		// currPos - current state of game.
		template<typename PIECE_T, typename RAY_DIRECTION_T>
		void emplace_back(BoardSquare begin, BoardSquare piece, BoardSquare end, BitBoard moveMask, const Position & currPos);

		void print(std::ostream & os = std::cout) const;

		MoveList::const_iterator find(Move move) const;

	private:
	};

	template<typename PIECE_T>
	void MoveList::emplace_back(Move move, const Position & currPos)
	{
		// 1.) --- Copy move and position to back of container ---
		// Specify base class to prevent infinite recursion
		MovePositionPair & pair = std::vector<MovePositionPair>::emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		pair.position.move<PIECE_T>(move);

#ifdef _DEBUG
		// 3.) --- Make sure it was a legal move ---
		if (AttackChecker::isKingAttacked(pair.position.board(), false)) {
			printError(move, __FUNCTION__);

			this->pop_back(); // Illegal Move
		}
#endif // _DEBUG
	}

	template<typename PIECE_T, typename RAY_DIRECTION_T>
	void MoveList::emplace_back(
		BoardSquare begin, 
		BoardSquare piece, 
		BoardSquare end, 
		BitBoard moveMask, 
		const Position & currPos)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a Ray");

		BoardSquare bs = begin;
		while (true) {
			if (moveMask[bs] == 1)	this->emplace_back<PIECE_T>(Move{ piece, bs }, currPos);

			if (RAY_DIRECTION_T::wouldBeInBounds(bs))	bs = RAY_DIRECTION_T::move(bs);
			else break;
		}
	}

} // namespace forge