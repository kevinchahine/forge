#pragma once

#include "Position.h"

#include <iostream>

#include <torch/csrc/api/include/torch/data/transforms/tensor.h>

// https://pytorch.org/cppdocs/notes/tensor_basics.html		for efficient tensor access

namespace forge
{
	class FeatureExtractor
	{
	public:
		void init(const Position& pos);

		void extractMaterial(torch::Tensor& slice);
		void extractMobility(torch::Tensor& slice);
		void extractAttacked(torch::Tensor& slice);

	public:
		// Counts the number of pieces standing in the direction of DIRECTION_T.
		// Example, if DIRECTION_T is directions::Horizontal and 4 pieces exists on the same
		// file as origin, then this method returns 4
		template<typename DIRECTION_T>
		size_t countOccupied(BoardSquare origin) const;

		// Determines if atleast 1 square in the direction of DIRECTION_T contains another piece.
		// Example, if DIRECTION_T is directions::Horizontal and a piece exists on the same
		// file as origin, then this method returns true
		// Excludes the piece that stands on origin.
		///template<typename DIRECTION_T>
		///bool isOccupied(BoardSquare origin) const;
			
		// Searches for an occupied square starting from 'start' and searching
		// in the direction of RAY_DIRECTION_T.
		// returns coordinates of occupied square if one is found.
		// returns an invalid BoardSquare if no occupied square is found before the edge of the board.
		template<typename RAY_DIRECTION_T>
		BoardSquare findOccupied(BoardSquare start) const;

		template<typename RAY_DIRECTION_T>
		bool isAttackPossible(BoardSquare victim) const;
		
		// Searches for a true attacker.
		// Starting from the victim square, searches in direction of RAY_DIRECTION_T for an attacker.
		// Returns coordinates of the attacker if one is found. Otherwise returns an invalid BoardSquare.
		//template<typename RAY_DIRECTION_T>
		//BoardSquare findAttacker(BoardSquare victim) const;
		
		//template<typename RAY_DIRECTION_T>
		//BitBoard findDirectionalAttacks() const;
		//
		// Creates a BitBoard of all pieces which are under attack (victims) by their opponents (attackers)
		// A 1 means that piece is being attacked by a piece of a different color
		// Does not account for pinned attackers. A piece which is pinned is still considered an attacker even
		// if it can't move in that direction.
		// ex: // extract all our attacked pawns
		//	Position position;
		//  const Board & b = position.board();
		//	FeatureExtractor fe;
		//	fe.init(position);
		//	BitBoard attacked = fe.findAllAttacked();
		//	BitBoard ourAttackedPawns = board.pawns() & attacked & board.whites();	// assuming white is moving
		BitBoard findAllAttacked() const;

	public:
		static const size_t MATERIAL_FEATURES_SIZE = 13 * 64;
		static const size_t MOBILITY_FEATURES_SIZE = 12 * 64;
		static const size_t ATTACKED_FEATURES_SIZE = 13 * 64;

	protected:
		Board board;

		BitBoard ours;
		BitBoard theirs;
		BitBoard occupied;
		BitBoard empty;

		BitBoard ourKings;
		BitBoard ourQueens;
		BitBoard ourBishops;
		BitBoard ourKnights;
		BitBoard ourRooks;
		BitBoard ourPawns;

		BitBoard theirKings;
		BitBoard theirQueens;
		BitBoard theirBishops;
		BitBoard theirKnights;
		BitBoard theirRooks;
		BitBoard theirPawns;
	};

	template<typename DIRECTION_T>
	size_t FeatureExtractor::countOccupied(BoardSquare origin) const
	{
		BitBoard mask = BitBoard::mask<DIRECTION_T>(origin);

		// Which squares on DIRECTION_T are occupied
		mask = mask & occupied;

		return mask.count();
	}

	template<typename RAY_DIRECTION_T>
	BoardSquare FeatureExtractor::findOccupied(BoardSquare start) const
	{
		BoardSquare bs = start;

		// Go from 'start' to edge of board in direction of 'RAY_DIRECTION_T' and
		// look for the 1st occupied square.
		while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
			// Move one square in direction of 'RAY_DIRECTION_T'
			bs = RAY_DIRECTION_T::move(bs);

			// Is this square occupied?
			if (occupied[bs]) {
				// Yes. We have found our occupied square.
				return bs;	// must be a return
			}
			// else. keep looking
		}

		// At this point, we did not find an occupied square.
		bs.setAsInvalid();

		return bs;
	}

	template<typename LINEAR_DIRECTION_T>
	inline bool FeatureExtractor::isAttackPossible(BoardSquare victim) const
	{
		static_assert(
			std::is_base_of<directions::Linear, LINEAR_DIRECTION_T>(),
			"DIRECTION_T must be a Linear direction.");
	
		// What squares would an opponent ray pieces have to be 
		// standing on if 'victim' is to be attacked?
		BitBoard extents = BitBoard::mask<LINEAR_DIRECTION_T>(victim);
	
		// For an attack to be possible (but non necessarily definite):
		//	- atleast one of their ray pieces needs to be standing on extents, and
		//	- atleast one of our blockers needs to be somewhere on extents also.
	
		// Does extents contain any of their rays? 
		BitBoard overlap = extents & theirs & this->board().directionals<LINEAR_DIRECTION_T>();
	
		// Does extents contain any of our blockers?
		BitBoard possiblePins = extents & ours;
	
		// If we have atleast one ray attacker and atleast one blocker, then an attack is possible.
		return overlap.any() && possiblePins.any();
	}
	
	//template<typename RAY_DIRECTION_T>
	//BoardSquare FeatureExtractor::findAttacker(BoardSquare victim) const
	//{
	//	BoardSquare bs = victim;
	//	BitBoard possiblePinners = this->board.directionals<RAY_DIRECTION_T>() & theirs;
	//	BitBoard occupied = ours | theirs;
	//
	//	// 2.) --- Find pinner piece (attacker) ---
	//	while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
	//		bs = RAY_DIRECTION_T::move(bs);	// Called first in the loop to move from the pinned piece
	//
	//		// Whats is on this square?
	//		if (possiblePinners[bs]) {
	//			// Its a pinner. Yay.
	//			// This square contains one of their ray pieces that can attack 
	//			// in the direction of our piece ('pinned').
	//			break;
	//		}
	//		else if (occupied[bs]) {
	//			// This square contains either one of their pieces (not a possible pinner) or
	//			// this square contains one of our pieces which is blocking the pin.
	//			// A pin is not possible in this direction.
	//			bs.setAsInvalid();
	//			break;
	//		}
	//	}
	//
	//	return bs;
	//}
	
	//template<typename RAY_DIRECTION_T>
	//BitBoard FeatureExtractor::findDirectionalAttacks() const
	//{
	//	BitBoard pins;
	//
	//	BoardSquare pinned;
	//	for (pinned.row(0); pinned.row() < 8; pinned.row(pinned.row() + 1)) {
	//		for (pinned.col(0); pinned.col() < 8; pinned.col(pinned.col() + 1)) {
	//			// Make sure this square has a piece standing on it. We can't pin an empty square.
	//			if (occupied[pinned] && ) {
	//				// Look for a pinner starting from 'pinned' and going in the direction of RAY_DIRECTION_T
	//				BoardSquare pinner = this->findPin<RAY_DIRECTION_T>(pinned);
	//
	//				// Was a pinner found?
	//				if (pinner.isValid()) {
	//					// Yes a pinner was found
	//					pins[pinned] = 1;		// Say that this square is a pinned piece
	//				}
	//				else {
	//					// No this piece is not pinned in this direction
	//				}
	//			}
	//		}
	//	}
	//
	//	return pins;
	//}
} // namespace forge
