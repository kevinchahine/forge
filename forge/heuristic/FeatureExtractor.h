#pragma once

#include <forge/core/Position.h>
#include <forge/core/IntBoard.h>

#include <iostream>

// #include <torch/csrc/api/include/torch/data/transforms/tensor.h>
#include <torch/torch.h>

// https://pytorch.org/cppdocs/notes/tensor_basics.html		for efficient tensor access

namespace forge
{
	namespace heuristic
	{
		class FeatureExtractor
		{
		public:
			// Evaluates the position from the perspective of the specified player
			// ex:
			//	forWhite = true:	evaluates board with white peices on the bottom (white pawns move up)
			//	forWhite = false:	evaluates board with black pieces on the bottom (black pawns move up)
			void init(const Position& pos, bool forWhite);

			void extractMaterial(torch::Tensor& slice);
			void extractMobility(torch::Tensor& slice);
			void extractAttacked(torch::Tensor& slice);

		public:
			// Counts the number of pieces standing in the direction of DIRECTION_T.
			// Example, if DIRECTION_T is directions::Horizontal and 4 pieces exists on the same
			// file as origin, then this method returns 4
			template<typename DIRECTION_T>
			size_t countOccupied(BoardSquare origin) const;

			// Determines if atleast 1 square in the direction of DIRECTION_T contains another piece.
			// Example, if DIRECTION_T is directions::Horizontal and a piece exists on the same
			// file as origin, then this method returns true
			// Excludes the piece that stands on origin.
			///template<typename DIRECTION_T>
			///bool isOccupied(BoardSquare origin) const;

			// Searches for an occupied square starting from 'start' and searching
			// in the direction of RAY_DIRECTION_T.
			// returns coordinates of occupied square if one is found.
			// returns an invalid BoardSquare if no occupied square is found before the edge of the board.
			template<typename RAY_DIRECTION_T>
			BoardSquare findOccupied(BoardSquare start) const;

			template<typename RAY_DIRECTION_T>
			bool isAttackPossible(BoardSquare victim) const;

			// Searches for a true attacker.
			// Starting from the victim square, searches in direction of RAY_DIRECTION_T for an attacker.
			// Returns coordinates of the attacker if one is found. Otherwise returns an invalid BoardSquare.
			//template<typename RAY_DIRECTION_T>
			//BoardSquare findAttacker(BoardSquare victim) const;

			// Searches a dimension (row, column) for pieces that are being attacked by opponents
			// from another direction.
			// ex:
			//	BitBoard attacked = findAttacked<directions::Right, directions::Down>(BoardSquare{ 0, 0 });
			//  // This returns a bit board of all pieces which are being attacked from their right
			//  The search iterates the board starting from BoardSquare{ 0, 0 } and going down.
			template<typename RAY_TO_ATTACKER_T, typename RAY_PERPENDICULAR_T>
			BitBoard findAttacked(BoardSquare start) const;

			// Searches a dimension (row, column) for pieces that are being attacked by opponents
			// from another direction. Counts the number of attackers from that direction.
			// ex:
			//	BitBoard attacked = findAttacked<directions::Right, directions::Down>(BoardSquare{ 0, 0 });
			//  // This returns a bit board of all pieces which are being attacked from their right
			//  The search iterates the board starting from BoardSquare{ 0, 0 } and going down.
			template<typename RAY_TO_ATTACKER_T, typename RAY_PERPENDICULAR_T>
			void countAttacked(BoardSquare start, IntBoard& counts) const;

			// Creates a BitBoard of all pieces which are under attack (victims) by their opponents (attackers)
			// A 1 means that piece is being attacked by a piece of a different color
			// Does not account for pinned attackers. A piece which is pinned is still considered an attacker even
			// if it can't move in that direction.
			// ex: // extract all our attacked pawns
			//	Position position;
			//  const Board & b = position.board();
			//	FeatureExtractor fe;
			//	fe.init(position);
			//	BitBoard attacked = fe.findAllAttacked();
			//	BitBoard ourAttackedPawns = board.pawns() & attacked & board.whites();	// assuming white is moving
			BitBoard findAllAttacked() const;

			IntBoard countAllAttacked() const;

		public:
			static const size_t MATERIAL_FEATURES_SIZE = 13 * 64;
			static const size_t MOBILITY_FEATURES_SIZE = 12 * 64;
			static const size_t ATTACKED_FEATURES_SIZE = 12 * 64;
			static const size_t N_LAYERS = 13;//  Black/White (K, Q, B, N, R, P), Empty
			static const std::vector<int64_t> TENSOR_SAMPLE_SIZE;

		protected:
			Board board;

			BitBoard ours;
			BitBoard theirs;
			BitBoard occupied;
			BitBoard empty;

			BitBoard ourKings;
			BitBoard ourQueens;
			BitBoard ourBishops;
			BitBoard ourKnights;
			BitBoard ourRooks;
			BitBoard ourPawns;

			BitBoard theirKings;
			BitBoard theirQueens;
			BitBoard theirBishops;
			BitBoard theirKnights;
			BitBoard theirRooks;
			BitBoard theirPawns;
		};

		template<typename DIRECTION_T>
		size_t FeatureExtractor::countOccupied(BoardSquare origin) const
		{
			BitBoard mask = BitBoard::mask<DIRECTION_T>(origin);

			// Which squares on DIRECTION_T are occupied
			mask = mask & occupied;

			return mask.count();
		}

		template<typename RAY_DIRECTION_T>
		BoardSquare FeatureExtractor::findOccupied(BoardSquare start) const
		{
			BoardSquare bs = start;

			// Go from 'start' to edge of board in direction of 'RAY_DIRECTION_T' and
			// look for the 1st occupied square.
			while (RAY_DIRECTION_T::wouldBeInBounds(bs)) {
				// Move one square in direction of 'RAY_DIRECTION_T'
				bs = RAY_DIRECTION_T::move(bs);

				// Is this square occupied?
				if (occupied[bs]) {
					// Yes. We have found our occupied square.
					return bs;	// must be a return
				}
				// else. keep looking
			}

			// At this point, we did not find an occupied square.
			bs.setAsInvalid();

			return bs;
		}

		template<typename LINEAR_DIRECTION_T>
		inline bool FeatureExtractor::isAttackPossible(BoardSquare victim) const
		{
			static_assert(
				std::is_base_of<directions::Linear, LINEAR_DIRECTION_T>(),
				"DIRECTION_T must be a Linear direction.");

			// What squares would an opponent ray pieces have to be 
			// standing on if 'victim' is to be attacked?
			BitBoard extents = BitBoard::mask<LINEAR_DIRECTION_T>(victim);

			// For an attack to be possible (but non necessarily definite):
			//	- atleast one of their ray pieces needs to be standing on extents, and
			//	- atleast one of our blockers needs to be somewhere on extents also.

			// Does extents contain any of their rays? 
			//TODO: Put this back in !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! BitBoard directionals = this->board().directionals<LINEAR_DIRECTION_T>();
			BitBoard overlap = extents & theirs;// & this->board().directionals<LINEAR_DIRECTION_T>();

			// Does extents contain any of our blockers?
			BitBoard possiblePins = extents & ours;

			// If we have atleast one ray attacker and atleast one blocker, then an attack is possible.
			return overlap.any() && possiblePins.any();
		}

		template<typename RAY_TO_ATTACKER_T, typename RAY_PERPENDICULAR_T>
		BitBoard FeatureExtractor::findAttacked(BoardSquare start) const
		{
			BitBoard victims;

			// Ray pieces which can attack Left (or Horizontally)
			BitBoard aggressors = board.directionals<RAY_TO_ATTACKER_T>();
			BitBoard ourAggressors = ours & aggressors;
			BitBoard theirAggressors = theirs & aggressors;

			// origin iterates one size of the board
			BoardSquare origin = start;
			while (true) {
				// Coorinates of where a potential victim is standing. 
				BoardSquare victim = origin;

				while (victim.isValid()) {
					// Where is the next occupied square?
					BoardSquare occupied = findOccupied<RAY_TO_ATTACKER_T>(victim);

					// Was an occupied square found?
					if (occupied.isInValid()) {
						// No. There are no more pieces in this direction.
						break;
					}

					// Does origin hold one of our or their pieces?
					// TODO: Optimize: This can be rewritten as a single expression
					// victims[origin] = (ours[origin] && thierAggressors[occupied]) || (theirs[origin] && ourAggressors[occupied]);
					if (ours[victim]) {
						// origin holds one of our pieces.
						// Is the next occupied square an attacker?
						if (theirAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							victims[victim] = 1;
						}
					}
					else if (theirs[victim]) {
						// origin holds one of their pieces.
						// Is the next occupied square an attacker?
						if (ourAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							victims[victim] = 1;
						}
					}
					else {
						// origin is an empty square.
					}

					// Move origin to the next occupied square.
					// Skip the empty squares.
					victim = occupied;
				}

				if (RAY_PERPENDICULAR_T::wouldBeInBounds(origin))
					origin = RAY_PERPENDICULAR_T::move(origin);
				else
					break;
			}

			return victims;
		}

		template<typename RAY_TO_ATTACKER_T, typename RAY_PERPENDICULAR_T>
		void FeatureExtractor::countAttacked(BoardSquare start, IntBoard& counts) const
		{
			// Ray pieces which can attack Left (or Horizontally)
			BitBoard aggressors = board.directionals<RAY_TO_ATTACKER_T>();
			BitBoard ourAggressors = ours & aggressors;
			BitBoard theirAggressors = theirs & aggressors;

			// origin iterates one size of the board
			BoardSquare origin = start;
			while (true) {
				// Coorinates of where a potential victim is standing. 
				BoardSquare victim = origin;

				while (victim.isValid()) {
					// Where is the next occupied square?
					BoardSquare occupied = findOccupied<RAY_TO_ATTACKER_T>(victim);

					// Was an occupied square found?
					if (occupied.isInValid()) {
						// No. There are no more pieces in this direction.
						break;
					}

					// Does origin hold one of our or their pieces?
					// TODO: Optimize: This can be rewritten as a single expression
					// victims[origin] = (ours[origin] && thierAggressors[occupied]) || (theirs[origin] && ourAggressors[occupied]);
					if (ours[victim]) {
						// origin holds one of our pieces.
						// Is the next occupied square an attacker?
						if (theirAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							counts[victim] += 1;
						}
					}
					else if (theirs[victim]) {
						// origin holds one of their pieces.
						// Is the next occupied square an attacker?
						if (ourAggressors[occupied]) {
							// Yes. This means origin is being attacked.
							counts[victim] += 1;
						}
					}
					else {
						// origin is an empty square.
					}

					// Move origin to the next occupied square.
					// Skip the empty squares.
					victim = occupied;
				}

				if (RAY_PERPENDICULAR_T::wouldBeInBounds(origin))
					origin = RAY_PERPENDICULAR_T::move(origin);
				else
					break;
			}
		}
	} // namespace heuristic
} // namespace forge
