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

	void FeatureExtractor::extractMaterial(torch::Tensor & slice)
	{
		if (slice.dim() != 2) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with 1 row and " << MATERIAL_FEATURES_SIZE << " columns" << endl;
		}
		else if (slice.size(0) != 1) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with 1 row" << endl;
		} 
		else if (slice.size(1) != MATERIAL_FEATURES_SIZE) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with " << MATERIAL_FEATURES_SIZE << " columns" << endl;
		}
		
		auto accessor = slice.accessor<float, 2>();	// Good for CPU access
		
		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			accessor[0][64 * 0  + bit] = static_cast<float>(empty[bit]);
			accessor[0][64 * 1  + bit] = static_cast<float>(ourKings[bit]);
			accessor[0][64 * 2  + bit] = static_cast<float>(ourQueens[bit]);
			accessor[0][64 * 3  + bit] = static_cast<float>(ourBishops[bit]);
			accessor[0][64 * 4  + bit] = static_cast<float>(ourKnights[bit]);
			accessor[0][64 * 5  + bit] = static_cast<float>(ourRooks[bit]);
			accessor[0][64 * 6  + bit] = static_cast<float>(ourPawns[bit]);
			accessor[0][64 * 7  + bit] = static_cast<float>(theirKings[bit]);
			accessor[0][64 * 8  + bit] = static_cast<float>(theirQueens[bit]);
			accessor[0][64 * 9  + bit] = static_cast<float>(theirBishops[bit]);
			accessor[0][64 * 10 + bit] = static_cast<float>(theirKnights[bit]);
			accessor[0][64 * 11 + bit] = static_cast<float>(theirRooks[bit]);
			accessor[0][64 * 12 + bit] = static_cast<float>(theirPawns[bit]);
		}
	}

	void FeatureExtractor::extractMobility(torch::Tensor slice)
	{
		torch::Tensor features = torch::zeros({ 1, MOBILITY_FEATURES_SIZE });
	
		// TODO: something goes here
	}
} // namespace forge