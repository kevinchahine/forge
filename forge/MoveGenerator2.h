#pragma once

#include "Position.h"
#include "MoveList.h"

#include <iostream>

namespace forge
{
	class MoveGenerator2
	{
	private:
		// Clears fields
		// Should be called once before every move generation.
		void reset();

		// Evaluates most fields that will be used in later methods.
		// Should be called onces in generate(), right after reset().
		void preprocess(const Board & b, bool isWhitesTurn);

	public:

		MoveList & generate(const Position & pos);

	private:
	public:	// <-- Only for testing
		void generateThreats(const Board & b);

		void countChecks(const Board & b, bool isWhitesTurn);

		void genPinMoves(const Board & b, bool isWhitesTurn);
		
		// Used for forward checking to efficiently verify that a pin is possible before performing
		// expensive pin search calculations. 
		// This method only tells us if a pin is possible or definitely impossible. 
		// It does not tell us if a pin definitely exists.
		// RAY_DIRECTION_T - is the a ray direction that a pin is to be verified from. 
		//	See namespace forge::directions.
		// theirRays - Squares containing their ray pieces that can attack in direction of RAY_DIRECTION_T.
		//		ex: Rooks and Queens can attack Up, Down, Left and Right
		// ourBlockers - Squares of all our pieces that can block our King. In otherwords, all our pieces
		//		except our King.
		template<typename DIRECTION_T>
		bool isPinPossible(BitBoard theirRays, BitBoard ourBlockers);

		// Searches in some direction for our pieces that are pinned to our King. 
		// theirRays - ray pieces that can attack in given directions. 
		//		ex: If RAY_DIRECTION_T is Up, then theirRays should equals thier queens and rooks.
		// Returns coordinate of the pinned piece.
		template<typename RAY_DIRECTION_T>
		BoardSquare pinSearch(BitBoard theirRays);

		// Searches in some direction for a piece that might qualify as a pinned to its king.
		// If a candidite piece is found, then its coordinates are returned.
		// If a piece is not found, then an invalid BoardSquare is returned.
		// * See comments of pinSearch()
		template<typename RAY_DIRECTION_T>
		BoardSquare findPinned(BitBoard theirRays);

		// Searches in some direction for a piece that might qualify as a pinner to a king.
		// pinnedPiece - coordinate of a candidate piece that could be pinned to king.
		// If a pinner is found, then its coordinates are returned.
		// If a pinner is not found, then an invalid BoardSquare is returend.
		// * See comments of pinSearch()
		template<typename RAY_DIRECTION_T>
		BoardSquare findPinner(BitBoard theirRays, BoardSquare pinnedPiece);

	private:
		// The squares that our pieces stand on
		BitBoard ours;
		// The squares that their pieces stand on
		BitBoard theirs;

		// Coordinate of our King
		BoardSquare ourKing;
		// Coordinate of their King
		BoardSquare theirKing;

		// Our Rays (Queens, Rooks, Bishops)
		BitBoard ourRays;
		// Their Rays 
		BitBoard theirRays;

		// Our Diagonal Rays (Queens and Bishops)
		BitBoard ourDiagonals;
		// Their Diagonal Rays
		BitBoard theirDiagonals;

		// Our Lateral Rays (Queens and Rooks)
		BitBoard ourLaterals;
		// Their Lateral Rays
		BitBoard theirLaterals;

		// The squares that our pieces are attacking
		BitBoard ourThreats;
		// The squares that their pieces are attacking
		BitBoard theirThreats;

		// Our non-King pieces
		BitBoard ourBlockers;
		// Their non-King pieces
		BitBoard theirBlockers;

		// The squares of our pinned pieces that are pinned to our King
		// This BitBoard will be set from genPinMoves()
		// After calling genPinMoves(), the moves of these pieces will have been added to the MoveList
		BitBoard ourAbsolutePins;

		// List of legal moves
		MoveList legalMoves;
	};

	template<typename DIRECTION_T>
	bool MoveGenerator2::isPinPossible(BitBoard theirRays, BitBoard ourBlockers)
	{
		static_assert(
			std::is_base_of<directions::Linear, DIRECTION_T>(),
			"DIRECTION_T must be a Linear direction.");

		// What squares would an opponent ray pieces have to be 
		// standing on if a pin was to be possible.
		BitBoard extents = BitBoard::mask<DIRECTION_T>(ourKing);

		// For a pin to be possible (but non necessarily definite):
		//	- atleast one of their ray pieces needs to be standing on extents, and
		//	- atleast one of our blockers needs to be somewhere on extents also.
		// Hints for optimizations: 
		//	- if DIRECTION_T is a multi directional direction, (ex: Diagonal, Lateral), 
		//		it is better to set ourBlockers to all ones so that only theirRays are account. 
		//		Usually our blockers will be on and off the direction of theirRays giveing
		//		us many false positives.
		//	- if DIRECTION_T is a ray direction, then its best to have ourBlockers be all
		//		our non-King pieces as it is less common for our pieces to be on the same 
		//		ray as their ray pieces. When they are, a pin is possible.

		// Does extents contain any of their rays? 
		BitBoard overlap = extents & theirRays;

		// Does extents contain any of our blockers?
		ourBlockers = extents & ourBlockers;

		// If we have atleast one ray attacker and atleast one blocker, then a pin is possible.
		return overlap.any() && ourBlockers.any();
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::pinSearch(BitBoard theirRays)
	{
		static_assert(
			std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a Ray direction.");

		BoardSquare pinned = findPinned<RAY_DIRECTION_T>(theirRays);
		
		// Did we find a candidate for a pinned piece?
		if (pinned.isValid()) {
			// Yes, now lets see if one of their pieces is pinning it to our King.
			BoardSquare pinner = findPinner<RAY_DIRECTION_T>(theirRays, pinned);

			// Did we find a pinner?
			if (pinner.isInValid()) {
				// No. Then there is no pinned piece after all.
				pinned.setAsInvalid();
			}
		}

		return pinned;
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::findPinned(BitBoard theirRays)
	{
		BoardSquare pinnedPiece;
		BoardSquare bs = ourKing;

		// 1.) --- Find pinned piece (blocker) ---
		while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {

			bs = RAY_DIRECTION_T::move(bs); // Called first to skip the King

			// Whats on this square?
			if (theirs[bs]) {
				// This square contains one of their pieces.
				// Meaning a pin does not exist in this direction. 
				// It might be a check instead.
				pinnedPiece.setAsInvalid();
			}
			else if (ourBlockers[bs]) {
				// This square contain one of our blockers.
				// It might be the pinned piece we're looking for.
				pinnedPiece = bs;

				// We still need to see if this piece has an attacker.
				break;
			}
		}

		return pinnedPiece;
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::findPinner(BitBoard theirRays, BoardSquare pinnedPiece)
	{
		BoardSquare bs = pinnedPiece;

		// 2.) --- Find pinner piece (attacker) ---
		while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
			bs = RAY_DIRECTION_T::move(bs);	// Called first to move from the pinnedPiece
			
			// Whats on this square?
			if (theirRays[bs]) {
				// Yay. Pinner is found.
				// This square contains one of their ray pieces that can attack 
				// in the direction of the King.
				break;
			}
			else if (theirs[bs]) {
				// This square contains one of their other pieces.
				// A pin is not possible in this direction.
				bs.setAsInvalid();
				break;
			}
			else if (ourBlockers[bs]) {
				// This square contains one of our blockers.
				// A pin is not possible in this direction.
				bs.setAsInvalid();
				break;
			}
		}

		return bs;
	}
} // namespace forge