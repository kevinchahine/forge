#include "FeatureExtractor.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	void FeatureExtractor::init(const Position & pos)
	{
		// --- Alias ---
		Board b = (pos.moveCounter().isWhitesTurn() ? pos.board() : pos.board().rotated());

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
		Eigen::Tensor<float, 2> features(1, 12 * 64);
		
		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			features(1, 64 * 0  + bit) = this->ourKings[bit];
			features(1, 64 * 1  + bit) = this->ourQueens[bit];
			features(1, 64 * 2  + bit) = this->ourBishops[bit];
			features(1, 64 * 3  + bit) = this->ourKnights[bit];
			features(1, 64 * 4  + bit) = this->ourRooks[bit];
			features(1, 64 * 5  + bit) = this->ourPawns[bit];
			features(1, 64 * 6  + bit) = this->theirKings[bit];
			features(1, 64 * 7  + bit) = this->theirQueens[bit];
			features(1, 64 * 8  + bit) = this->theirBishops[bit];
			features(1, 64 * 9  + bit) = this->theirKnights[bit];
			features(1, 64 * 10 + bit) = this->theirRooks[bit];
			features(1, 64 * 11 + bit) = this->theirPawns[bit];
		}

		return features;
	}

	Eigen::Tensor<float, 2> FeatureExtractor::extractMobility()
	{
		Eigen::Tensor<float, 2> features(1, 12 * 64);

		

		return features;
	}
} // namespace forge