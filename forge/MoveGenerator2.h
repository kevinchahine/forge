#pragma once

#include "KingAttackers.h"
#include "Position.h"
#include "MoveList.h"
#include "Pin.h"

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
		void preprocess(const Position & pos);

	public:

		MoveList & generate(const Position & pos);

		const BitBoard & getThreats() const { return threats; }

	private:
	public:	// <-- Only for testing

		void genPinMoves(const Board & b, bool isWhitesTurn);
		
		// Make sure to only call on valid Pin objects.  
		// Generated Positions will be push to this->legalMoves
		template<typename RAY_DIRECTION_T>
		void genPinMovesFor(Pin pin);

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
		bool isPinPossible() const;

		// Searches in some direction for our pieces that are pinned to our King. 
		// theirRays - ray pieces that can attack in given directions. 
		//		ex: If RAY_DIRECTION_T is Up, then theirRays should equals thier queens and rooks.
		// Performs efficient forward checking to identify if a pin is possible before
		//	performing further expensive search.
		// Returns:
		//		first - coordinate of pinned
		//		second - coordinate of pinner
		template<typename RAY_DIRECTION_T>
		Pin pinSearch() const;

		// Searches in some direction for a piece that might qualify as a pinned to its king.
		// If a candidite piece is found, then its coordinates are returned.
		// If a piece is not found, then an invalid BoardSquare is returned.
		// * See comments of pinSearch()
		template<typename RAY_DIRECTION_T>
		BoardSquare findPinned() const;

		// Searches in some direction for a piece that might qualify as a pinner to a king.
		// pinnedPiece - coordinate of a candidate piece that could be pinned to king.
		// If a pinner is found, then its coordinates are returned.
		// If a pinner is not found, then an invalid BoardSquare is returend.
		// * See comments of pinSearch()
		template<typename RAY_DIRECTION_T>
		BoardSquare findPinner(BoardSquare pinnedPiece) const;

		// Given a direction and nothing else, this method will search for absolute pins and generate
		// legal moves for the pinned piece.  Moves are added to legalMoves.
		template<typename DIRECTION_T>
		void searchAndGeneratePins();

		// Generates King caputres and pushes.
		void genKingMoves();

		template <typename RAY_DIRECTION_T>
		inline void findAndMoveBlockers(const BoardSquare & blockableSquare)
		{
			// Find a piece that can move to this square and block the attacker.
			///BoardSquare block = MoveGenHelpers::findAttackingRay<RAY_DIRECTION_T>(
			///	blockableSquare,
			///	currPositionPtr->board(), 
			///	ours,
			///	theirs);
			///
			///// Was a blocker piece found? Make sure it isn't occupied in a pin.
			///if (block.isValid() && !ourAbsolutePins[block]) {
			///	// Yes, an unpinned piece was found to block the attacker.
			///	legalMoves.emplace_back<pieces::Piece>(Move{ block, blockableSquare }, *currPositionPtr);
			///}
		}

		// Generates moves that block attackers from attacking the King
		// and moves that capture pieces that attack the King.
		// Should not be called with searchAndGeneratePins() otherwise some
		// moves will be generated twice.
		void genBlockAndCaptureMoves(const KingAttacker & attacker);

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

		// The squares that their pieces are attacking.
		// Our King cannot move to any of these square.
		BitBoard threats;

		// Our non-King pieces
		BitBoard ourBlockers;
		// Their non-King pieces
		BitBoard theirBlockers;

		// The squares of our pinned pieces that are pinned to our King
		// This BitBoard will be set from genPinMoves()
		// After calling genPinMoves(), the moves of these pieces will have been added to the MoveList
		BitBoard ourAbsolutePins;

		const Position * currPositionPtr = nullptr;
		
		// List of legal moves
		MoveList legalMoves;
	};
} // namespace forge

#include "MoveGenerator2_Definitions.h"