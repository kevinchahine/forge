#include "Material.h"

using namespace std;

namespace forge
{
	namespace heuristic
	{
		heuristic_t Material::eval(const Position& pos)
		{
			heuristic_t val =
				// --- Material ---
				imbalance<pieces::Queen>(pos) * m_weights.queenMaterial +
				imbalance<pieces::Bishop>(pos) * m_weights.bishopMaterial +
				imbalance<pieces::Knight>(pos) * m_weights.knightMaterial +
				imbalance<pieces::Rook>(pos) * m_weights.rookMaterial +
				imbalance<pieces::Pawn>(pos) * m_weights.pawnMaterial +
				0;	// Placeholder for nicer syntax
			
			return val;
		}

		heuristic_t Material::eval(const Position& pos, bool whiteIsSearching)
		{
			heuristic_t val = eval(pos);

			return (whiteIsSearching ? val : -val);
		}

		unique_ptr<Base> Material::clone() const
		{
			return make_unique<Material>(*this);
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

		void Material::print(const Position& pos, std::ostream& os) const
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