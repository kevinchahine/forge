#include "FeatureExtractor.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	void FeatureExtractor::init(const Position & pos)
	{
		// --- Alias ---
		b = (pos.moveCounter().isWhitesTurn() ? pos.board() : pos.board().rotated());

		ours = b.whites();
		theirs = b.blacks();

		ourKings = ours & b.kings();
		ourQueens = ours & b.queens();
		ourBishops = ours & b.bishops();
		ourKnights = ours & b.knights();
		ourRooks = ours & b.rooks();
		ourPawns = ours & b.pawns();

		theirKings = theirs & b.kings();
		theirQueens = theirs & b.queens();
		theirBishops = theirs & b.bishops();
		theirKnights = theirs & b.knights();
		theirRooks = theirs & b.rooks();
		theirPawns = theirs & b.pawns();
	}

	Eigen::Tensor<float, 2> FeatureExtractor::extractMaterial()
	{
		Eigen::Tensor<float, 2> features(1, MATERIAL_FEATURES_SIZE + 1);
		
		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			features(1, 64 * 0  + bit) = empty[bit];
			features(1, 64 * 1  + bit) = ourKings[bit];
			features(1, 64 * 2  + bit) = ourQueens[bit];
			features(1, 64 * 3  + bit) = ourBishops[bit];
			features(1, 64 * 4  + bit) = ourKnights[bit];
			features(1, 64 * 5  + bit) = ourRooks[bit];
			features(1, 64 * 6  + bit) = ourPawns[bit];
			features(1, 64 * 7  + bit) = theirKings[bit];
			features(1, 64 * 8  + bit) = theirQueens[bit];
			features(1, 64 * 9  + bit) = theirBishops[bit];
			features(1, 64 * 10 + bit) = theirKnights[bit];
			features(1, 64 * 11 + bit) = theirRooks[bit];
			features(1, 64 * 12 + bit) = theirPawns[bit];
		}

		return features;
	}

	Eigen::Tensor<float, 2> FeatureExtractor::extractMobility()
	{
		Eigen::Tensor<float, 2> features(1, MOBILITY_FEATURES_SIZE);

		

		return features;
	}
} // namespace forge