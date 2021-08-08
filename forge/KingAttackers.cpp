#include "KingAttackers.h"

#include "Attackers.h"

namespace forge
{
	void KingAttackers::print(const Board & board, std::ostream & os) const
	{
		os << this->size() << " attackers: ";

		for (const auto & attacker : *this) {
			os << board.at(attacker.square) << ' ' << attacker.square << ' ';
		}
	}

	// -------------------------------- STATIC METHODS ------------------------
	
	// !!!VERY IMPORTANT: IF RAY_DIRECTION_T is a Lateral direction, always set 'isRayAttacking' to true
	//	so that promotions to Rook/Queens are accounted for.
	template<typename RAY_DIRECTION_T>
	inline void pushIfRayIsAttackingKing(
		const Board & board,
		const BoardSquare & ourKing,
		const BitBoard & ours,
		const BitBoard & theirs,
		KingAttackers & pair,
		bool & isRayAttacking) {
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"This method must only be called for a Ray direction");

		// Have we already found a Ray attacker is some direction. 
		// Remember: 2 Diagonal attacks are impossible.
		// 2 Lateral attacks ARE Possible but only from ROOK/QUEEN promotions.
		// !!! 'isRayAttacking' should always be set to false when 'RAY_DIRECTION_T' is a Lateral direction.
		if (!isRayAttacking) {
			// 1.) --- Search in this direction for a Ray attacker. ---
			// Attacker could be a Queen, Rook, or Bishop.
			BoardSquare attacker = Attackers::findAttackingRay<RAY_DIRECTION_T>(
				ourKing,
				board,
				theirs);

			// 2.) --- Did we find a Ray attacker? ---
			if (attacker.isValid()) {
				// Yes. It's coordinates are stored in 'attacker'
				pair.push_back_rays<RAY_DIRECTION_T>(ourKing, attacker);
				isRayAttacking = true;
			}
		}
	}

	// Determines if 'ourKing' is being attacked by some knight.
	// That knight may or may not be on a square. 
	// The square will be determined by 'ourKing' and 'KNIGHT_DIRECTION_T'.
	template<typename KNIGHT_DIRECTION_T>
	inline void pushIfKnightIsAttackingKing(
		const BoardSquare & ourKing,
		const BitBoard & theirKnights,
		KingAttackers & pair,
		bool & isKnightAttack)
	{
		static_assert(std::is_base_of<directions::LShape, KNIGHT_DIRECTION_T>(),
			"This method must only be called for a LShape (Knight) direction");

		// First of allToFen, determine if an attacking Knight has already been found.
		// Remember that only 1 Knight can attack the King at a time in a Chess game.
		if (!isKnightAttack)
		{
			// Is this move on the board or out-of-bounds?
			if (KNIGHT_DIRECTION_T::wouldBeInBounds(ourKing)) {
				// Yes. It is in-bounds.

				// On which square does must the knight exist?
				BoardSquare knight = KNIGHT_DIRECTION_T::move(ourKing);

				// Does a knight exist on this square?
				if (theirKnights[knight]) {
					// Yes it exists. Also, it is attacking our King.
					pair.push_back_non_rays<KNIGHT_DIRECTION_T>(knight);
					isKnightAttack = true;
				}
			}
		} // if (isKnightAttack)
	}

	KingAttackers KingAttackers::findKingAttackers(const Board & board, BoardSquare ourKing, BitBoard theirs, BitBoard ours)
	{
		KingAttackers pair;		// initialized to invalid attackers

		bool isKnightAttack = false;
		bool isDiagonalAttack = false;
		bool isLateralAttack = false;
		bool isPawnAttack = false;

		// TODO: Optimize (HUGE):
		// Remember that the only possible combination of 2 attackers are as follows:
		//	- Queen + non-King
		//		- Queen + Queen (promoted, Lateral)
		//		- Queen + Knight
		//		- Queen + Bishop (Queen Lateral)
		//		- Queen + Rook (Queen Diagonal)
		//		- Queen + Pawn (Queen Vertical)
		//	- Bishop + Knight
		//	- Bishop + Rook
		//	- Rook + Knight
		//	- Rook + Pawn
		//	- Rook + Rook (promoted)
		//	
		// Impossible combinations:
		//	- King + anything
		//	- 2 Knights
		//	- 2 Diagonals
		//		- 2 Bishops
		//		- Queen (Diagonal) + Bishop
		//		- 2 Queens (Diagonal)
		//	- 2 Pawns
		//	- Bishop + Pawn

		// --- Look for attackers of each piece type ---

		// 1-1.) --- Knights ---
		{
			BitBoard captureMask = pieces::Knight::captureMask(ourKing);
			BitBoard theirKnights = captureMask & board.knights() & theirs;

			// Is our King attacked by a Knight? (Only 1 Knight can attack King at a time)
			if (theirKnights.any()) {
				// Yes. A Knight is attacking our King. Lets find this Knight?
				// TODO: Optimize: Only one Knight can attack a King at a time. Once 1 is found
				//	we can skip the rest.
				pushIfKnightIsAttackingKing<directions::Knight0>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight1>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight2>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight3>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight4>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight5>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight6>(ourKing, theirKnights, pair, isKnightAttack);
				pushIfKnightIsAttackingKing<directions::Knight7>(ourKing, theirKnights, pair, isKnightAttack);
			} // end Knights
		}

		// 1-2.) --- Bishops/Queens (Diagonals) ---
		{
			BitBoard captureMask = pieces::Bishop::captureMask(ourKing);
			BitBoard theirDiagonals = captureMask & board.diagonals() & theirs;

			// Is our King attacked by some diagonal piece? (Only 1 Diagonal piece can attack at a time)
			if (theirDiagonals.any()) {
				// A Diagonal attack is possible. Lets look in more detail.
				pushIfRayIsAttackingKing<directions::UR>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::UL>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::DR>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::DL>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
			}
		} // end Diagonals

		// --- Look Ahead ---
		if (pair.isFull()) {
			return pair;
		}

		// 1-3.) --- Rooks/Queens (Laterals) ---
		{
			BitBoard captureMask = pieces::Rook::captureMask(ourKing);
			BitBoard theirLaterals = captureMask & board.laterals() & theirs;

			// Is our King attacked by some lateral piece?
			if (theirLaterals.any()) {
				// A Lateral attack is possible. Lets look in more detail.

				pushIfRayIsAttackingKing<directions::Up>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::Down>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::Left>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
				pushIfRayIsAttackingKing<directions::Right>(board, ourKing, ours, theirs, pair, isDiagonalAttack);
			}
		} // end Laterals

		// --- Look Ahead ---
		if (pair.isFull()) {
			return pair;
		}

		// 1-4.) --- Pawns ---
		if (board.isWhite(ourKing)) {
			// Our King is White. There Pawns are Black.

			BitBoard captureMask = pieces::WhitePawn::captureMask(ourKing);
			BitBoard theirPawns = captureMask & board.pawns() & board.blacks();

			// Is it possible for one of their pawns to attack our King?
			if (theirPawns.any()) {
				// It is possible.
				if (!ourKing.isTopRank()) {
					if (!ourKing.isLeftFile() && board.isPawn(ourKing.upLeftOne())) {
						// Pawn is attacking our King
						pair.push_back_non_rays<directions::UL>(ourKing.upLeftOne());
						isPawnAttack = true;
					}
					if (!ourKing.isRightFile() && board.isPawn(ourKing.upRightOne())) {
						// Pawn is attacking our King
						pair.push_back_non_rays<directions::UR>(ourKing.upRightOne());
						isPawnAttack = true;
					}
				}
			}
		}
		else {
			// Our King is Black. There Pawns are White.

			BitBoard captureMask = pieces::BlackPawn::captureMask(ourKing);
			BitBoard theirPawns = captureMask & board.pawns() & board.whites();

			// Is it possible for one of their pawns to attack our King?
			if (theirPawns.any()) {
				if (!ourKing.isBotRank()) {
					if (!ourKing.isLeftFile() && board.isPawn(ourKing.downLeftOne())) {
						// Pawn is attacking our King
						pair.push_back_non_rays<directions::DL>(ourKing.downLeftOne());
						isPawnAttack = true;
					}
					if (!ourKing.isRightFile() && board.isPawn(ourKing.downRightOne())) {
						// Pawn is attacking our King
						pair.push_back_non_rays<directions::DR>(ourKing.downRightOne());
						isPawnAttack = true;
					}
				}
			}
		}

		// 1-5.) --- Kings (not possible. Kings can't attack Kings) ---
		// Nothing to do here

		return pair;
	}

} // namespace forge