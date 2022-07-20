#include "ApplePieWeights.h"

namespace forge
{
	bool ApplePieWeights::operator==(const ApplePieWeights & rhs) const
	{
		return 
			// --- Material ---
			queenMaterial == rhs.queenMaterial &&
			rookMaterial == rhs.rookMaterial &&
			bishopMaterial == rhs.bishopMaterial &&
			knightMaterial == rhs.knightMaterial &&
			pawnMaterial == rhs.pawnMaterial &&
			true;
	}
} // namespace forge