#pragma once

#include "HeuristicBase.h"
#include "ApplePieWeights.h"

#include <array>

namespace forge
{
	// Evaluation function to approximate the favorability of a given
	//	position. Evaluation is used for entire game, unlike Stockfish
	//	where evaluations change based on phase of the game.
	// Based on this: https://hxim.github.io/Stockfish-Evaluation-Guide/
	//	Evaluates base on the following criteria:
	//		Material:
	//			- #Queens
	//			- #Rooks
	//			- #Bishops
	//			- #Knights
	//			- #Pawns
	//			- Bishops Pair
	//			- Opposite Bishops: 1 bishop each of opposite color
	//		Mobility:
	//			- Mobility of Queens
	//			- Mobility of Rooks
	//			- Mobility of Bishops
	//			- Mobility of Knights
	//			- Mobility of Pawns
	//			- Mobility of Kings
	//		Piece Square Table Bonus:
	//			- Each piece that sits on a square gets the bonus of that square
	//			- Each type of piece has its own bonus table.
	//		Defended Pieces:
	//			- Each defended piece gets a bonus based on its type multiplied by 
	//				the number of defenders it has.
	//			- nDefenders * defendedPieceTypeWeight
	//			- If a piece is not defended its defense bonus will be 0
	//			- #Queen Defenders * defendedQueenWeight
	//			- #Rook Defenders * defendedRookWeight
	//			- #Bishop Defenders * defendedBishopWeight
	//			- #Knight Defenders * defendedKnightWeight
	//			- #Pawn Defenders * defendedPawnWeight
	//		Protected King:
	//			- Number of pieces that surround (Block) King from allToFen sides by friendly pieces
	//		Attacking Pieces:
	//			- Number of times that each piece attacks an opponents pieces.
	//			- nAttacks * attackerPieceTypeWeight
	//		Pawns:
	//			- Pawns Rank: The further up the board the better (even for black)
	//			- # Connected Pawns: Number of times each pawn "attacks" it's friendly pawns
	//			- 
	class ApplePieHeuristic : public HeuristicBase
	{
	public:
		// Calculates evaluation interms of the white player
		virtual heuristic_t eval(const Position & pos) override;

		virtual std::unique_ptr<HeuristicBase> clone() const override;

		virtual std::string name() const override { return "ApplePieHeuristic"; }

		virtual void print(const Position & pos, std::ostream & os = std::cout) const override;

		ApplePieWeights & weights() { return m_weights; }
		const ApplePieWeights & weights() const { return m_weights; }

	protected:
		ApplePieWeights m_weights;
	};
} // namespace forge