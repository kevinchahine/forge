#pragma once

#include "MoveGenerator2.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	void MoveGenerator2::genPinMovesFor(Pin pin)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		// Mark the pinned piece as an absolut pin so that later we don't 
		// generate its free moves.
		ourAbsolutePins[pin.pinned] = 1;	

		// Mark the bits between the King (exclusive) and Attacker (inclusive)
		BitBoard battleGround;/// =
			///BitBoard::mask<directions::reverse<RAY_DIRECTION_T>()>(pin.pinner, pin.pinned);

		// 2.) --- Calculate Push and Capture Masks ---
		BitBoard pushMask;
		BitBoard captureMask;

		// TODO: OPTIMIZE: The order of these 'ifs' can be adjusted to have
		// the most common pinned pieces first
		const Board & b = currPositionPtr->board();
		if (b.isPawn(pin.pinned)) {
			if (b.isWhite(pin.pinned)) {
				pushMask = pieces::WhitePawn::pushMask(pin.pinned);
				captureMask = pieces::WhitePawn::captureMask(pin.pinned);
			}
			else {
				pushMask = pieces::BlackPawn::pushMask(pin.pinned);
				captureMask = pieces::BlackPawn::captureMask(pin.pinned);
			}
		}
		else if (b.isKnight(pin.pinned)) {
			pushMask = pieces::Knight::pushMask(pin.pinned);
			captureMask = pieces::Knight::captureMask(pin.pinned);
		} 
		else if (b.isBishop(pin.pinned)) {
			pushMask = pieces::Bishop::pushMask(pin.pinned);
			captureMask = pieces::Bishop::captureMask(pin.pinned);
		}
		else if (b.isRook(pin.pinned)) {
			pushMask = pieces::Rook::pushMask(pin.pinned);
			captureMask = pieces::Rook::captureMask(pin.pinned);
		}
		else if (b.isQueen(pin.pinned)) {
			pushMask = pieces::Queen::pushMask(pin.pinned);
			captureMask = pieces::Queen::captureMask(pin.pinned);
		}
#ifdef _DEBUG
		else {
			cout << "Error: " << __LINE__ << " line " << __FILE__
				<< ": pinned square should be a non-Empty, non-King square\n";
		}
#endif // _DEBUG

		pushMask &= battleGround & (~theirs);
		captureMask &= battleGround & (theirs);

		// 3.) --- Actually generate some moves ---
		// --- Forward Checking ---
		if (pushMask.any()) {
			//legalMoves.
		}
		if (captureMask.any()) {
		}
	}

	template<typename DIRECTION_T>
	bool MoveGenerator2::isPinPossible()
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
		//		Our blockers might be in one direction (ex: Right) from the King while
		//		one of their theirRays might be in another direction (ex: Up).
		//		In this case a pin is not possible.
		//		Because of this it is better to set ourBlockers to all ones
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
	Pin MoveGenerator2::pinSearch()
	{
		static_assert(
			std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a Ray direction.");

		// Coordinates of pinned and pinner pieces
		Pin pin;

		// Is it possible for a pin to occur in this direction?
		if (isPinPossible<RAY_DIRECTION_T>()) {
			pin.pinned = findPinned<RAY_DIRECTION_T>();

			// Did we find a candidate for a pinned piece?
			if (pin.pinned.isValid()) {
				// Yes, now lets see if one of their pieces are pinning it to our King.
				pin.pinner = findPinner<RAY_DIRECTION_T>(pin.pinned);

				// Did we find a pinner?
				if (pin.pinner.isInValid()) {
					// No. Then there is no pinned piece after all.
					pin.pinned.setAsInvalid();
				}
			}
		}

		return pin;
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::findPinned()
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
	BoardSquare MoveGenerator2::findPinner(BoardSquare pinnedPiece)
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