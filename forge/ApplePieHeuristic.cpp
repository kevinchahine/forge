#include "ApplePieHeuristic.h"

using namespace std;

namespace forge
{
	heuristic_t ApplePieHeuristic::eval(const Position & pos) const
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

	template<typename F, typename W>
	class PrintHelper {
	public:
		PrintHelper() = default;
		PrintHelper(const F & feature, const W & weight) :
			feature(feature),
			weight(weight) {}
		PrintHelper(const PrintHelper &) = default;
		PrintHelper(PrintHelper &&) noexcept = default;
		virtual ~PrintHelper() noexcept = default;
		PrintHelper & operator=(const PrintHelper &) = default;
		PrintHelper & operator=(PrintHelper &&) noexcept = default;

		friend ostream & operator<<(ostream & os, const PrintHelper & ph) {
			float w = static_cast<float>(ph.weight) / 100.0f;
			os << setw(10) << ph.feature << " x " << w << " = " << ph.feature * w;

			return os;
		}

		F feature;
		W weight;
	};

	void ApplePieHeuristic::print(const Position & pos, std::ostream & os) const
	{
		os
			<< "Piece Imbalances:\n"
			<< "\tQueen  " << PrintHelper(imbalance<Piece::QUEEN>(pos), m_weights.queenMaterial) << endl
			<< "\tBishop " << PrintHelper(imbalance<Piece::BISHOP>(pos), m_weights.bishopMaterial) << endl
			<< "\tKnight " << PrintHelper(imbalance<Piece::KNIGHT>(pos), m_weights.knightMaterial) << endl
			<< "\tRook   " << PrintHelper(imbalance<Piece::ROOK>(pos), m_weights.rookMaterial) << endl
			<< "\tPawn   " << PrintHelper(imbalance<Piece::PAWN>(pos), m_weights.pawnMaterial) << endl

			<< "Total = " << 
				imbalance<Piece::QUEEN>(pos) * m_weights.queenMaterial +
				imbalance<Piece::BISHOP>(pos) * m_weights.bishopMaterial +
				imbalance<Piece::KNIGHT>(pos) * m_weights.knightMaterial +
				imbalance<Piece::ROOK>(pos) * m_weights.rookMaterial +
				imbalance<Piece::PAWN>(pos) * m_weights.pawnMaterial
			<< endl;
	}
} // namespace forge