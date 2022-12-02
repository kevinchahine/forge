#include "FeatureExtractor.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	namespace heuristic
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
			cout << __FILE__ << " line " << __LINE__ << " stop using this method" << endl;

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

		void FeatureExtractor::init(const Position& pos, bool maximizeWhite)
		{
			// Orient board so that the moving player (ours) is on the bottom 
			// and waiting player (theirs) is on the top
			board = (maximizeWhite ? pos.board() : pos.board().rotated());

			// *** At this point, colors should be interms of ours/theirs ***
			// *** OUR   pieces ALWAYS move UP ***
			// *** THEIR pieces ALWAYS move DOWN ***
			// *** Now the maximizing players pieces will be on the bottom moving up ***
			// *** And the minimizing players pieces will be on the top moving down ***
			// *** ex:
			//	If its whites is maximizing, our pieces will be white moving up, their pieces will be black moving down
			//	It its blacks is maximizing, our pieces will be black moving up, their pieces will be white moving down

			ours = (maximizeWhite ? board.whites() : board.blacks());
			theirs = (maximizeWhite ? board.blacks() : board.whites());
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
				///accessor[0][64 * 0 + bit] = static_cast<float>(empty[bit]);
				accessor[0][64 * 0 + bit] = static_cast<float>(ourKings[bit]);
				accessor[0][64 * 1 + bit] = static_cast<float>(ourQueens[bit]);
				accessor[0][64 * 2 + bit] = static_cast<float>(ourBishops[bit]);
				accessor[0][64 * 3 + bit] = static_cast<float>(ourKnights[bit]);
				accessor[0][64 * 4 + bit] = static_cast<float>(ourRooks[bit]);
				accessor[0][64 * 5 + bit] = static_cast<float>(ourPawns[bit]);
				accessor[0][64 * 6 + bit] = static_cast<float>(theirKings[bit]);
				accessor[0][64 * 7 + bit] = static_cast<float>(theirQueens[bit]);
				accessor[0][64 * 8 + bit] = static_cast<float>(theirBishops[bit]);
				accessor[0][64 * 9 + bit] = static_cast<float>(theirKnights[bit]);
				accessor[0][64 * 10 + bit] = static_cast<float>(theirRooks[bit]);
				accessor[0][64 * 11 + bit] = static_cast<float>(theirPawns[bit]);
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
			IntBoard attackedPieces = countAllAttacked();

			IntBoard ourAttackedKings = attackedPieces & ourKings;
			IntBoard ourAttackedQueens = attackedPieces & ourQueens;
			IntBoard ourAttackedBishops = attackedPieces & ourBishops;
			IntBoard ourAttackedKnights = attackedPieces & ourKnights;
			IntBoard ourAttackedRooks = attackedPieces & ourRooks;
			IntBoard ourAttackedPawns = attackedPieces & ourPawns;

			IntBoard theirAttackedKings = attackedPieces & theirKings;
			IntBoard theirAttackedQueens = attackedPieces & theirQueens;
			IntBoard theirAttackedBishops = attackedPieces & theirBishops;
			IntBoard theirAttackedKnights = attackedPieces & theirKnights;
			IntBoard theirAttackedRooks = attackedPieces & theirRooks;
			IntBoard theirAttackedPawns = attackedPieces & theirPawns;

			auto accessor = slice.accessor<float, 2>();	// Good for CPU access

			for (size_t row = 0; row < 8; row++) {
				for (size_t col = 0; col < 8; col++) {
					// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
					accessor[0][64 * 0 + (row * 8 + col)] = static_cast<float>(ourAttackedKings[row][col]);
					accessor[0][64 * 1 + (row * 8 + col)] = static_cast<float>(ourAttackedQueens[row][col]);
					accessor[0][64 * 2 + (row * 8 + col)] = static_cast<float>(ourAttackedBishops[row][col]);
					accessor[0][64 * 3 + (row * 8 + col)] = static_cast<float>(ourAttackedKnights[row][col]);
					accessor[0][64 * 4 + (row * 8 + col)] = static_cast<float>(ourAttackedRooks[row][col]);
					accessor[0][64 * 5 + (row * 8 + col)] = static_cast<float>(ourAttackedPawns[row][col]);
					accessor[0][64 * 6 + (row * 8 + col)] = static_cast<float>(theirAttackedKings[row][col]);
					accessor[0][64 * 7 + (row * 8 + col)] = static_cast<float>(theirAttackedQueens[row][col]);
					accessor[0][64 * 8 + (row * 8 + col)] = static_cast<float>(theirAttackedBishops[row][col]);
					accessor[0][64 * 9 + (row * 8 + col)] = static_cast<float>(theirAttackedKnights[row][col]);
					accessor[0][64 * 10 + (row * 8 + col)] = static_cast<float>(theirAttackedRooks[row][col]);
					accessor[0][64 * 11 + (row * 8 + col)] = static_cast<float>(theirAttackedPawns[row][col]);
				}
			}
		}

		BitBoard FeatureExtractor::findAllAttacked() const
		{
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

		IntBoard FeatureExtractor::countAllAttacked() const
		{
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