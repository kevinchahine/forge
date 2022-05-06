#include "FeatureExtractor.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	bool checkTensorSize(const torch::Tensor& slice, size_t nColumns)
	{
		const bool GOOD_SIZE = true;
		const bool BAD_SIZE = false;

		if (slice.dim() != 2) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with 1 row and " << nColumns << " columns" << endl;
			return BAD_SIZE;
		}
		else if (slice.size(0) != 1) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with 1 row" << endl;
			return BAD_SIZE;
		}
		else if (slice.size(1) != nColumns) {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << endl
				<< "\tslice must be a 2D tensor with " << nColumns << " columns" << endl;
			return BAD_SIZE;
		}

		return GOOD_SIZE;
	}

	void FeatureExtractor::init(const Position& pos)
	{
		// Orient board so that the moving player (ours) is on the bottom 
		// and waiting player (theirs) is on the top
		bool isWhitesTurn = pos.moveCounter().isWhitesTurn();
		board = (isWhitesTurn ? pos.board() : pos.board().rotated());

		// *** At this point, colors should be interms of ours/theirs ***
		// *** OUR   pieces ALWAYS move UP ***
		// *** THEIR pieces ALWAYS move DOWN ***
		// *** Now the moving players pieces will be on the bottom moving up ***
		// *** And the waiting players pieces will be on the top moving down ***
		// *** ex:
		//	If its whites turn, our pieces will be white moving up, their pieces will be black moving down
		//	It its blacks turn, our pieces will be black moving up, their pieces will be white moving down
		
		ours = (isWhitesTurn ? board.whites() : board.blacks());
		theirs = (isWhitesTurn ? board.blacks() : board.whites());
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

	void FeatureExtractor::extractMaterial(torch::Tensor& slice)
	{
		if (checkTensorSize(slice, MATERIAL_FEATURES_SIZE) == false)
			return;

		auto accessor = slice.accessor<float, 2>();	// Good for CPU access

		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			accessor[0][64 * 0 + bit] = static_cast<float>(empty[bit]);
			accessor[0][64 * 1 + bit] = static_cast<float>(ourKings[bit]);
			accessor[0][64 * 2 + bit] = static_cast<float>(ourQueens[bit]);
			accessor[0][64 * 3 + bit] = static_cast<float>(ourBishops[bit]);
			accessor[0][64 * 4 + bit] = static_cast<float>(ourKnights[bit]);
			accessor[0][64 * 5 + bit] = static_cast<float>(ourRooks[bit]);
			accessor[0][64 * 6 + bit] = static_cast<float>(ourPawns[bit]);
			accessor[0][64 * 7 + bit] = static_cast<float>(theirKings[bit]);
			accessor[0][64 * 8 + bit] = static_cast<float>(theirQueens[bit]);
			accessor[0][64 * 9 + bit] = static_cast<float>(theirBishops[bit]);
			accessor[0][64 * 10 + bit] = static_cast<float>(theirKnights[bit]);
			accessor[0][64 * 11 + bit] = static_cast<float>(theirRooks[bit]);
			accessor[0][64 * 12 + bit] = static_cast<float>(theirPawns[bit]);
		}
	}

	void FeatureExtractor::extractMobility(torch::Tensor& slice)
	{
		if (checkTensorSize(slice, MOBILITY_FEATURES_SIZE) == false)
			return;

		// TODO: something goes here
	}

	// nodes:
	//  1th  64: ourAttackedKings	
	//	2st  64: ourAttackedQueens
	//	3nd  64: ourAttackedBishops
	//	4rd  64: ourAttackedKnights
	//  5th  64: ourAttackedRooks
	//  6th  64: ourAttackedPawns
	//  7th  64: theirAttackedKings
	//	8th  64: theirAttackedQueens
	//	9th  64: theirAttackedBishops
	//	10th 64: theirAttackedKnights
	//  11th 64: theirAttackedRooks
	//  12th 64: theirAttackedPawns
	void FeatureExtractor::extractAttacked(torch::Tensor& slice)
	{
		if (checkTensorSize(slice, ATTACKED_FEATURES_SIZE) == false)
			return;

		// --- Attacked Pieces ---
		BitBoard attackedPieces = findAllAttacked();

		BitBoard ourAttackedQueens;
		BitBoard ourAttackedBishops;
		BitBoard ourAttackedKnights;
		BitBoard ourAttackedRooks;
		BitBoard ourAttackedPawns;

		BitBoard theirAttackedQueens;
		BitBoard theirAttackedBishops;
		BitBoard theirAttackedKnights;
		BitBoard theirAttackedRooks;
		BitBoard theirAttackedPawns;

		auto accessor = slice.accessor<float, 2>();	// Good for CPU access

		// TODO: something goes here
	}

	BitBoard FeatureExtractor::findAllAttacked() const
	{
		BitBoard victims;

		// --- Up ---
		// --- Down ---
		// --- Right ---
		{
			// Ray pieces which can attack Left (or Horizontally)
			BitBoard aggressors = board.directionals<directions::Horizontal>();
			BitBoard ourAggressors = ours & aggressors;
			BitBoard theirAggressors = theirs & aggressors;

			// Go down each row and look for attacks.
			for (int row = 0; row < 8; row++) {
				// TODO: Optimize: Add in this optimization
				// First lets see if its possible for an attack to occur on this row from the right
				//if (isAttackPossible<directions::Horizontal>(BoardSquare{ row, 0 })) {
				//
				//}

				// Go accross this row and look for attacks comming from the right.
				BoardSquare origin{ row, 0 };

				while (origin.isValid()) {
					// Where is the next occupied square?
					BoardSquare occupied = findOccupied<directions::Right>(origin);

					cout << "row:\t" << row << "\torigin:\t" << origin << "\toccupied:\t" << occupied << endl;

					// Was an occupied square found?
					if (occupied.isInValid()) {
						// No. There are no more pieces in this direction.
						break;
					}

					// Does origin hold one of our or their pieces?
					// TODO: Optimize: This can be rewritten as a single expression
					// victims[origin] = (ours[origin] && thierAggressors[occupied]) || (theirs[origin] && ourAggressors[occupied]);
					if (ours[origin]) {
						// origin holds one of our pieces.
						// Is the next occupied square an attacker?
						if (theirAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							victims[origin] = 1;
						}
					}
					else if (theirs[origin]) {
						// origin holds one of their pieces.
						// Is the next occupied square an attacker?
						if (ourAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							victims[origin] = 1;
						}
					}
					else {
						// origin is an empty square.
					}

					// Move origin to the next occupied square.
					// Skip the empty squares.
					origin = occupied;
				}
			}
		}

		// --- Left ---
		// --- UR ---
		// --- UL ---
		// --- DL ---
		// --- DR ---
		// --- Knights ---
		// --- Pawns ---
		// --- Kings ---

		return victims;
	}

	// ------------------------- HELPER METHODS ----------------------------------

	//BitBoard FeatureExtractor::findAllAttacks() const
	//{
	//	// BitBoard of all pinned pieces no matter the direction
	//	BitBoard pins;
	//
	//	for (int8_t row = 0; row < 8; row++) {
	//		for (int8_t col = 0; col < 8; col++) {
	//
	//		}
	//	}
	//
	//	return pins;
	//}
} // namespace forge