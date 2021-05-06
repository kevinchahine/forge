#include "ApplePieHeuristic.h"

using namespace std;

namespace forge
{
	heuristic_t ApplePieHeuristic::eval(const Position & pos)
	{
		return
		// --- Material ---
		imbalance<Piece::QUEEN>(pos) * m_weights.queenMaterial +
		imbalance<Piece::BISHOP>(pos) * m_weights.bishopMaterial +
		imbalance<Piece::KNIGHT>(pos) * m_weights.knightMaterial +
		imbalance<Piece::ROOK>(pos) * m_weights.rookMaterial +
		imbalance<Piece::PAWN>(pos) * m_weights.pawnMaterial +

		///// --- 
		///heuristic_t bishopPair = 0;
		///heuristic_t oppositeBishop = 0;
		///
		///// --- Mobility ---
		///// Based on how many spaces each piece can move.
		///heuristic_t queenMobility = 0;
		///heuristic_t rookMobility = 0;
		///heuristic_t bishopMobility = 0;
		///heuristic_t knightMobility = 0;
		///heuristic_t pawnMobility = 0;
		///heuristic_t kingMobility = 0;
		///
		///// --- Piece Square Table Bonus ---
		///PositionTable queenPSTB;
		///PositionTable rookPSTB;
		///PositionTable bishopPSTB;
		///PositionTable knightPSTB;
		///PositionTable pawnPSTB;
		///PositionTable kingPSTB;
		///
		///// --- Defended Pieces ---
		///heuristic_t queensDefended = 0;
		///heuristic_t rooksDefended = 0;
		///heuristic_t bishopsDefended = 0;
		///heuristic_t knightsDefended = 0;
		///heuristic_t pawnsDefended = 0;
		///heuristic_t kingsDefended = 0;

		0;	// Placeholder for nicer syntax
	}

	unique_ptr<HeuristicBase> ApplePieHeuristic::clone() const
	{
		return make_unique<ApplePieHeuristic>(*this);
	}
} // namespace forge