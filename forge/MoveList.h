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
		void pushIfLegalQBNMove(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		void pushIfLegalQBNCapture(const Position & currPos, Move move);

		// * See comment for pushIfLegalQBNMove
		void pushIfLegalWhitePawnMove(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		void pushIfLegalBlackPawnMove(const Position & currPos, Move move);

		// * See comment for pushIfLegalQBNMove
		void pushIfLegalWhitePawnCapture(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		void pushIfLegalBlackPawnCapture(const Position & currPos, Move move);

		// * See comment for pushIfLegalQBNMove
		// Accounts for promotions involving captures also
		void pushIfLegalWhitePawnPromotion(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		// Accounts for promotions involving captures also
		void pushIfLegalBlackPawnPromotion(const Position & currPos, Move move);

		// * See comment for pushIfLegalQBNMove
		void pushIfLegalRookMove(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		void pushIfLegalRookCapture(const Position & currPos, Move move);

		// * See comment for pushIfLegalQBNMove
		void pushIfLegalKingMove(const Position & currPos, Move move);
		// * See comment for pushIfLegalQBNMove
		void pushIfLegalKingCapture(const Position & currPos, Move move);

		void print(std::ostream & os = std::cout) const;

	private:
	};

} // namespace forge