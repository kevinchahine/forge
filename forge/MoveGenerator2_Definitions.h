#pragma once

#include "MoveGenerator2.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	void MoveGenerator2::genPinMovesFor(Pin pin)
	{
		const Board & b = currPositionPtr->board();
		const pieces::Piece piece = b.at(pin.pinned);

		// 0.) --- Make sure direction is a Ray
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be a ray direction");

		// 1.) --- Mark the pinned piece ---
		// Mark as an absolut pin so that later we don't generate its free moves.
		ourAbsolutePins[pin.pinned] = 1;	

		// 2.) --- Mark the bits between the King (exclusive) and Attacker (inclusive) ---
		BitBoard battleGround = BitBoard::mask<RAY_DIRECTION_T>(ourKing, pin.pinner);
		battleGround[ourKing] = 0;		// Exclude King
		battleGround[pin.pinner] = 1;	// Include pinner

		// 3.) --- Calculate Push and Capture Masks ---
		// Squares pinned piece can push to (same for non-Pawns)
		// Squares pinned piece can capture on (same for non-Pawns)
		BitBoard pushMask = piece.pushMask(pin.pinned);			// Squares pinned piece can push to 
		BitBoard captureMask = piece.captureMask(pin.pinned);	// Squares pinned piece can capture on
		
		// 4.) --- Overlap masks with battleGround and their pieces ---
		pushMask &= battleGround & (~theirs);		// Should contain 1s where pinned can push to between King and pinner
		captureMask &= battleGround & (theirs);		// Should contain a 1 on pinner or all zeros

		// 5.) --- Actually generate some moves already ---
		// --- Look ahead: Are any push moves possible? ---
		if (pushMask.any()) {
			// Yes a push move is possible

			// What piece is moving?
			if (piece.isPawn()) {
				if (piece.isWhite())
					legalMoves.emplace_back<pieces::WhitePawn, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
				else
					legalMoves.emplace_back<pieces::BlackPawn, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
			}
			if (piece.isQueen()) 
				legalMoves.emplace_back<pieces::Queen, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
			if (piece.isBishop()) 
				legalMoves.emplace_back<pieces::Bishop, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
			if (piece.isKnight())
				legalMoves.emplace_back<pieces::Knight, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
			if (piece.isRook())
				legalMoves.emplace_back<pieces::Rook, RAY_DIRECTION_T>(ourKing, pin.pinned, pin.pinner, pushMask, *currPositionPtr);
			// No need to do King because, Kings are never pinned.
		}
		// --- Look ahead: Are any capture moves possible? ---
		if (captureMask.any()) {
			// Yes a capture move is possible

			// What piece is moving?
			if (piece.isPawn()) {
				if (piece.isWhite())
					legalMoves.emplace_back<pieces::WhitePawn>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
				else
					legalMoves.emplace_back<pieces::BlackPawn>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
			}
			if (piece.isQueen())
				legalMoves.emplace_back<pieces::Queen>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
			if (piece.isBishop())
				legalMoves.emplace_back<pieces::Bishop>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
			if (piece.isKnight())
				legalMoves.emplace_back<pieces::Knight>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
			if (piece.isRook())
				legalMoves.emplace_back<pieces::Rook>(Move{ pin.pinned, pin.pinner }, *currPositionPtr);
			// No need to do King because, Kings are never pinned.
		}
	}

	template<typename DIRECTION_T>
	bool MoveGenerator2::isPinPossible() const
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
		BitBoard overlap = extents & theirs & currPositionPtr->board().directionals<DIRECTION_T>();

		// Does extents contain any of our blockers?
		BitBoard possiblePins = extents & ourBlockers;

		// If we have atleast one ray attacker and atleast one blocker, then a pin is possible.
		return overlap.any() && possiblePins.any();
	}

	template<typename RAY_DIRECTION_T>
	Pin MoveGenerator2::pinSearch() const
	{
		// Make sure direction is a Ray Direction
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
			}
		}

		return pin;
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::findPinned() const
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
				pinnedPiece.setAsValid();

				// We still need to see if this piece has an attacker.
				break;
			}
		}

		return pinnedPiece;
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare MoveGenerator2::findPinner(BoardSquare pinnedPiece) const
	{
		BoardSquare bs = pinnedPiece;
		BitBoard possiblePinners = currPositionPtr->board().directionals<RAY_DIRECTION_T>() & theirs;

		// 2.) --- Find pinner piece (attacker) ---
		while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
			bs = RAY_DIRECTION_T::move(bs);	// Called first to move from the pinnedPiece

			// Whats on this square?
			if (possiblePinners[bs]) {
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

	template<typename DIRECTION_T>
	void MoveGenerator2::searchAndGeneratePins()
	{
		// Find the pinned and pinner pieces for the absolute pin
		Pin pin = pinSearch<DIRECTION_T>();

		cout << pin.pinned << (pin.pinned.isValid() ? " valid" : " invalid")
			<< ' ' << pin.pinner << (pin.pinner.isValid() ? " valid" : " invalid")
			<< " is "
			<< (pin.isValid() ? "" : "NOT") << " a pin in "
			<< typeid(DIRECTION_T).name() << " direction." << endl;

		// If a valid absolute pin was found, generate its legal moves
		if (pin.isValid()) { genPinMovesFor<DIRECTION_T>(pin); }
	}
} // namespace forge