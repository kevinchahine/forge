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

	torch::Tensor FeatureExtractor::extractMaterial()
	{
		torch::Tensor features = torch::zeros({ MATERIAL_FEATURES_SIZE });
		
		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			features[64 * 0  + bit] = static_cast<float>(empty[bit]);
			features[64 * 1  + bit] = static_cast<float>(ourKings[bit]);
			features[64 * 2  + bit] = static_cast<float>(ourQueens[bit]);
			features[64 * 3  + bit] = static_cast<float>(ourBishops[bit]);
			features[64 * 4  + bit] = static_cast<float>(ourKnights[bit]);
			features[64 * 5  + bit] = static_cast<float>(ourRooks[bit]);
			features[64 * 6  + bit] = static_cast<float>(ourPawns[bit]);
			features[64 * 7  + bit] = static_cast<float>(theirKings[bit]);
			features[64 * 8  + bit] = static_cast<float>(theirQueens[bit]);
			features[64 * 9  + bit] = static_cast<float>(theirBishops[bit]);
			features[64 * 10 + bit] = static_cast<float>(theirKnights[bit]);
			features[64 * 11 + bit] = static_cast<float>(theirRooks[bit]);
			features[64 * 12 + bit] = static_cast<float>(theirPawns[bit]);
		}
	
		return features;
	}

	torch::Tensor FeatureExtractor::extractMobility()
	{
		torch::Tensor features = torch::zeros({ 1, MOBILITY_FEATURES_SIZE });
	
		// TODO: something goes here
	
		return features;
	}
} // namespace forge