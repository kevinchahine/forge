#pragma once

#include "MovePositionPair.h"

#include <vector>

namespace forge
{
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

		// Purpose: Pushes move to MoveList but only if move/capture was legal:
		//	1.) Push move and Position to MoveList
		//	2.) Check to see if move was illegal
		//	3.) If illegal, pops off Move and Position 
		// Warning: May reallocate underlying container causing iteraters
		//	and pointers of MoveList to be invalid regardless of wether
		//	move was legal or not.
		// Accounts for promotions involving captures also
		template<typename PIECE_T>
		void emplace_back(Move move, const Position & currPos);

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

} // namespace forge