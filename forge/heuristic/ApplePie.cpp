#include "ApplePie.h"

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t ApplePie::eval(const Position& pos)
		{
			// Calculate evaluation in terms of white player. 
			// Higher and positive numbers mean whites winning.
			// *** See return statement below

			heuristic_t val =
				// --- Material ---
				imbalance<pieces::Queen>(pos) * m_weights.queenMaterial +
				imbalance<pieces::Bishop>(pos) * m_weights.bishopMaterial +
				imbalance<pieces::Knight>(pos) * m_weights.knightMaterial +
				imbalance<pieces::Rook>(pos) * m_weights.rookMaterial +
				imbalance<pieces::Pawn>(pos) * m_weights.pawnMaterial +

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

			//!!!wrong: Recalculate evaluation so that score is in terms of moving player.
			//!!!wrong: The higher and more positive the number, the better it is for the moving player
			//!!!wrong: regardless of whether the moving player is white or black
			return val;
		}

		unique_ptr<Base> ApplePie::clone() const
		{
			return make_unique<ApplePie>(*this);
		}

		// TODO: Can we rewrite this as a single function
		template<typename F, typename W>
		class PrintHelper {
		public:
			PrintHelper() = default;
			PrintHelper(const F& feature, const W& weight) :
				feature(feature),
				weight(weight) {}
			PrintHelper(const PrintHelper&) = default;
			PrintHelper(PrintHelper&&) noexcept = default;
			virtual ~PrintHelper() noexcept = default;
			PrintHelper& operator=(const PrintHelper&) = default;
			PrintHelper& operator=(PrintHelper&&) noexcept = default;

			friend ostream& operator<<(ostream& os, const PrintHelper& ph) {
				float w = static_cast<float>(ph.weight) / 100.0f;
				os << setw(10) << ph.feature << " x " << w << " = " << ph.feature * w;

				return os;
			}

			F feature;
			W weight;
		};

		void ApplePie::print(const Position& pos, std::ostream& os) const
		{
			os
				<< "Piece Imbalances:\n"
				<< "\tQueen  " << PrintHelper<int, heuristic_t>(imbalance<pieces::Queen>(pos), m_weights.queenMaterial) << endl
				<< "\tBishop " << PrintHelper<int, heuristic_t>(imbalance<pieces::Bishop>(pos), m_weights.bishopMaterial) << endl
				<< "\tKnight " << PrintHelper<int, heuristic_t>(imbalance<pieces::Knight>(pos), m_weights.knightMaterial) << endl
				<< "\tRook   " << PrintHelper<int, heuristic_t>(imbalance<pieces::Rook>(pos), m_weights.rookMaterial) << endl
				<< "\tPawn   " << PrintHelper<int, heuristic_t>(imbalance<pieces::Pawn>(pos), m_weights.pawnMaterial) << endl

				<< "Total = " <<
				imbalance<pieces::Queen>(pos) * m_weights.queenMaterial +
				imbalance<pieces::Bishop>(pos) * m_weights.bishopMaterial +
				imbalance<pieces::Knight>(pos) * m_weights.knightMaterial +
				imbalance<pieces::Rook>(pos) * m_weights.rookMaterial +
				imbalance<pieces::Pawn>(pos) * m_weights.pawnMaterial
				<< endl;
		}
	} // namespace heuristic
} // namespace forge