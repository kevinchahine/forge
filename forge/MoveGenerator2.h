#pragma once

#include "KingAttackers.h"
#include "Position.h"
#include "MoveList.h"
#include "Attackers.h"
#include "Threats.h"
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

		// Searches for Pins and generates legal moves for those pieces but
		// only if 'searchOnly' is set to false.
		// If 'searchOnly' is set to true, then Pins are searched and recorded without 
		// any moves being generated.
		// When King is attacked exacly once, 'searchOnly' should be true.
		// Otherwise 'searchOnly' should be false.
		void genPinMoves(const Board & b, bool isWhitesTurn, bool searchOnly);
		
		// Make sure to only call on valid Pin objects.  
		// Generated Positions will be push to this->legalMoves
		// If 'searchOnly' is set to true, then Pins are searched and recorded without 
		// any moves being generated.
		template<typename RAY_DIRECTION_T>
		void genPinMovesFor(Pin pin, bool searchOnly);

		// Used for forward checking to efficiently verify that a pin is possible before performing
		// expensive pin search calculations. 
		// This method only tells us if a pin is possible or definitely impossible. 
		// It does not tell us if a pin definitely exists.
		// RAY_DIRECTION_T - is the a ray direction that a pin is to be verified from. 
		//	See namespace forge::directions.
		// theirRays - Squares containing their ray pieces that can attack in direction of RAY_DIRECTION_T.
		//		ex: Rooks and Queens can attack Up, Down, Left and Right
		// ourBlockers - Squares of allToFen our pieces that can block our King. In otherwords, allToFen our pieces
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
		// When 'searchOnly' is true, pins are only searched and their moves are not generated.
		template<typename DIRECTION_T>
		void searchAndGeneratePins(bool searchOnly);

		// Generates King caputres and pushes.
		void genKingMoves();

		// Generates moves that block attackers from attacking the King
		// and moves that capture pieces that attack the King.
		// Should not be called with searchAndGeneratePins() otherwise some
		// moves will be generated twice.
		void genBlockAndCaptureMoves(const KingAttacker & attacker);

		void genPawnBlockAndCapture(const KingAttacker& attacker, BitBoard captureMask);
		void genLateralBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask);
		void genDiagonalBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask);
		void genKnightBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask);

		void genFreeMoves();

		void genFreePawnMoves(BoardSquare pawn);
		void genFreeRookMoves(BoardSquare rook);
		void genFreeKnightMoves(BoardSquare knight);
		void genFreeBishopMoves(BoardSquare bishop);
		void genFreeQueenMoves(BoardSquare queen);

	private:
		// The squares occupied by pieces of both colors
		BitBoard occupied;
		// The squares that are empty
		BitBoard empty;

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