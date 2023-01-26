#include "FeatureExtractor.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	namespace heuristic
	{
		// ---------------------------- STATIC FIELDS -------------------------

		const std::vector<int64_t> FeatureExtractor::TENSOR_SAMPLE_SIZE = { 1, N_LAYERS, 8, 8 };

		// ---------------------------- HELPER FUNCTIONS ----------------------

		bool checkTensorSize(const torch::Tensor & slice) {
			// --- Alias ---
			const bool GOOD_SIZE = true;
			const bool BAD_SIZE = false;

			// --- Check Dimensions ---
			if (slice.sizes() != FeatureExtractor::TENSOR_SAMPLE_SIZE) {
				cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
					<< "\tTensor must be a 4D tensor of shape " << FeatureExtractor::TENSOR_SAMPLE_SIZE << endl
					<< "\tInstead got a Tensor of shape " << slice.sizes() << endl;
				cin.get();
				return BAD_SIZE;
			}

			return GOOD_SIZE;
		}

		void FeatureExtractor::init(const Position & pos, bool forWhite) {
			// Orient board so that the moving player (ours) is on the bottom 
			// and waiting player (theirs) is on the top
			board = (forWhite ? pos.board() : pos.board().rotated());

			// *** At this point, colors should be interms of ours/theirs ***
			// *** OUR   pieces ALWAYS move UP ***
			// *** THEIR pieces ALWAYS move DOWN ***
			// *** Now the maximizing players pieces will be on the bottom moving up ***
			// *** And the minimizing players pieces will be on the top moving down ***
			// *** ex:
			//	If its whites is maximizing, our pieces will be white moving up, their pieces will be black moving down
			//	It its blacks is maximizing, our pieces will be black moving up, their pieces will be white moving down

			ours = (forWhite ? board.whites() : board.blacks());
			theirs = (forWhite ? board.blacks() : board.whites());
			occupied = board.occupied();
			empty = board.empty();

			ourKings = ours & board.kings();
			ourQueens = ours & board.queens();
			ourBishops = ours & board.bishops();
			ourKnights = ours & board.knights();
			ourRooks = ours & board.rooks();
			ourPawns = ours & board.pawns();

			theirKings = theirs & board.kings();
			theirQueens = theirs & board.queens();
			theirBishops = theirs & board.bishops();
			theirKnights = theirs & board.knights();
			theirRooks = theirs & board.rooks();
			theirPawns = theirs & board.pawns();
		}

		// ------------------------- EXTRACT METHODS ---------------------------------

		void FeatureExtractor::extractMaterial(torch::Tensor & slice) {
			if (checkTensorSize(slice) == false)
				return;

			auto accessor = slice.accessor<float, 4>();	// Good for fast CPU access

			for (size_t bit = 0; bit < 64; bit++) {
				int64_t row = bit / 8;
				int64_t col = bit % 8;

				accessor[0][0][row][col] = static_cast<float>(ourKings[bit]);
				accessor[0][1][row][col] = static_cast<float>(ourQueens[bit]);
				accessor[0][2][row][col] = static_cast<float>(ourBishops[bit]);
				accessor[0][3][row][col] = static_cast<float>(ourKnights[bit]);
				accessor[0][4][row][col] = static_cast<float>(ourRooks[bit]);
				accessor[0][5][row][col] = static_cast<float>(ourPawns[bit]);
				accessor[0][6][row][col] = static_cast<float>(theirKings[bit]);
				accessor[0][7][row][col] = static_cast<float>(theirQueens[bit]);
				accessor[0][8][row][col] = static_cast<float>(theirBishops[bit]);
				accessor[0][9][row][col] = static_cast<float>(theirKnights[bit]);
				accessor[0][10][row][col] = static_cast<float>(theirRooks[bit]);
				accessor[0][11][row][col] = static_cast<float>(theirPawns[bit]);
				accessor[0][12][row][col] = static_cast<float>(empty[bit]);
			}
		}

		BitBoard FeatureExtractor::findAllAttacked() const {
			BitBoard victims;

			// --- Up ---
			victims |= findAttacked<directions::Up, directions::Right>(BoardSquare{ 7, 0 });

			// --- Down ---
			victims |= findAttacked<directions::Down, directions::Right>(BoardSquare{ 0, 0 });

			// --- Left ---
			victims |= findAttacked<directions::Left, directions::Down>(BoardSquare{ 0, 7 });

			// --- Right ---
			victims |= findAttacked<directions::Right, directions::Down>(BoardSquare{ 0, 0 });

			// --- UR ---

			// --- UL ---

			// --- DL ---

			// --- DR ---

			// --- Knights ---
			// --- Pawns ---
			// --- Kings ---

			return victims;
		}

		IntBoard FeatureExtractor::countAllAttacked() const {
			IntBoard counts;

			// --- Up ---
			countAttacked<directions::Up, directions::Right>(BoardSquare{ 7, 0 }, counts);

			// --- Down ---
			countAttacked<directions::Down, directions::Right>(BoardSquare{ 0, 0 }, counts);

			// --- Left ---
			countAttacked<directions::Left, directions::Down>(BoardSquare{ 0, 7 }, counts);

			// --- Right ---
			countAttacked<directions::Right, directions::Down>(BoardSquare{ 0, 0 }, counts);

			// --- UR ---

			// --- UL ---

			// --- DL ---

			// --- DR ---

			// --- Knights ---
			// --- Pawns ---
			// --- Kings ---

			return counts;
		}
	} // namespace heuristic
} // namespace forge