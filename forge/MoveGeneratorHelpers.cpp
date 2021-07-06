#include "MoveGeneratorHelpers.h"

using namespace std;

namespace forge
{
	BitBoard MoveGenHelpers::genThreats(const Board & board, BitBoard attackers)
	{
		BitBoard threats;

		// obstacles is made up of all pieces (including their king) excluding our king
		BitBoard obstacles = board.occupied() & ~(board.kings() & ~attackers);

		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare attacker{ row, col };

				if (attackers[attacker]) {
					threats |= genThreats(board, attacker, obstacles);
				}
			}
		}

		return threats;
	}

	BitBoard MoveGenHelpers::genThreats(const Board & board, BoardSquare attacker, BitBoard obstacles)
	{
		BitBoard threats;

		// Placed in order of most common to least common pieces
		if (board.isPawn(attacker)) {
			if (board.isWhite(attacker)) threats = genThreatsFor<pieces::WhitePawn>(attacker, obstacles);
			else threats = genThreatsFor<pieces::BlackPawn>(attacker, obstacles);
		}
		else if (board.isKnight(attacker)) threats = genThreatsFor<pieces::Knight>(attacker, obstacles);
		else if (board.isBishop(attacker)) threats = genThreatsFor<pieces::Bishop>(attacker, obstacles);
		else if (board.isRook(attacker)) threats = genThreatsFor<pieces::Rook>(attacker, obstacles);
		else if (board.isQueen(attacker)) threats = genThreatsFor<pieces::Queen>(attacker, obstacles);
		else if (board.isKing(attacker)) threats = genThreatsFor<pieces::King>(attacker, obstacles);
#ifdef _DEBUG
		else {
			cout << "Error: " << __FILE__ << " line " << __LINE__ << "\n\t"
				<< "Piece could not be identified.  Is this square even occupied\n";
		}
#endif // _DEBUG

		return threats;
	}

	AttackerPair MoveGenHelpers::findKingAttackers(
		const Board & board,
		BoardSquare ourKing,
		BitBoard theirs,
		BitBoard ours)
	{
		AttackerPair pair;		// initialized to invalid attackers

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
				// Yes. A Knight is attacking our King.

				// --- Knight 0 ---
				if (ourKing.isKnight0InBounds() && theirKnights[ourKing.knight0()]) {
					pair.push_back(ourKing.knight0());
					isKnightAttack = true;
				}
				// --- Knight 1 ---
				else if (ourKing.isKnight1InBounds() && theirKnights[ourKing.knight1()]) {
					pair.push_back(ourKing.knight1());
					isKnightAttack = true;
				}
				// --- Knight 2 ---
				else if (ourKing.isKnight2InBounds() && theirKnights[ourKing.knight2()]) {
					pair.push_back(ourKing.knight2());	// Attacker found
					isKnightAttack = true;
				}
				// --- Knight 3 ---
				else if (ourKing.isKnight3InBounds() && theirKnights[ourKing.knight3()]) {
					pair.push_back(ourKing.knight3());	// Attacker found
					isKnightAttack = true;
				}
				// --- Knight 4 ---
				else if (ourKing.isKnight4InBounds() && theirKnights[ourKing.knight4()]) {
					pair.push_back(ourKing.knight4());	// Attacker found
					isKnightAttack = true;
				}
				// --- Knight 5 ---
				else if (ourKing.isKnight5InBounds() && theirKnights[ourKing.knight5()]) {
					pair.push_back(ourKing.knight5());	// Attacker found
					isKnightAttack = true;
				}
				// --- Knight 6 ---
				else if (ourKing.isKnight6InBounds() && theirKnights[ourKing.knight6()]) {
					pair.push_back(ourKing.knight6());	// Attacker found
					isKnightAttack = true;
				}
				// --- Knight 7 ---
				else if (ourKing.isKnight7InBounds() && theirKnights[ourKing.knight7()]) {
					pair.push_back(ourKing.knight7());	// Attacker found
					isKnightAttack = true;
				}
			} // end Knights
		}

		// 1-2.) --- Bishops/Queens (Diagonals) ---
		{
			BitBoard captureMask = pieces::Bishop::captureMask(ourKing);
			BitBoard theirDiagonals = captureMask & board.diagonals() & theirs;

			// Is our King attacked by some diagonal piece? (Only 1 Diagonal piece can attack at a time)
			if (theirDiagonals.any()) {
				// Maybe it is attacked by a diagonal

				BoardSquare attacker;

				// --- UR ---
				attacker = findAttacker<directions::UR>(ourKing, board, theirs, ours);
				if (attacker.isValid()) {
					pair.push_back(attacker);
					isDiagonalAttack = true;
				}
				else {
					// --- UL ---
					attacker = findAttacker<directions::UL>(ourKing, board, theirs, ours);
					if (attacker.isValid()) {
						pair.push_back(attacker);
						isDiagonalAttack = true;
					}
					else {
						// --- DL ---
						attacker = findAttacker<directions::DL>(ourKing, board, theirs, ours);
						if (attacker.isValid()) {
							pair.push_back(attacker);
							isDiagonalAttack = true;
						}
						else {
							// --- DR ---
							attacker = findAttacker<directions::DR>(ourKing, board, theirs, ours);
							if (attacker.isValid()) {
								pair.push_back(attacker);
								isDiagonalAttack = true;
							}
						}
					}
				}
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
				// Maybe it is attacked by a lateral

				BoardSquare attacker;

				if (pair.isNotFull()) {
					// --- Up ---
					attacker = findAttacker<directions::Up>(ourKing, board, theirs, ours);
					if (attacker.isValid()) {
						pair.push_back(attacker);
						isLateralAttack = true;
					}

					if (pair.isNotFull()) {
						// --- Down ---
						attacker = findAttacker<directions::Down>(ourKing, board, theirs, ours);
						if (attacker.isValid()) {
							pair.push_back(attacker);
							isLateralAttack = true;
						}

						if (pair.isNotFull()) {
							// --- Left ---
							attacker = findAttacker<directions::Left>(ourKing, board, theirs, ours);
							if (attacker.isValid()) {
								pair.push_back(attacker);
								isLateralAttack = true;
							}

							if (pair.isNotFull()) {
								// --- Right ---
								attacker = findAttacker<directions::Right>(ourKing, board, theirs, ours);
								if (attacker.isValid()) {
									pair.push_back(attacker);
									isLateralAttack = true;
								}
							} // end Right
						} // end Left
					} // end Down
				} // end Up
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
						pair.push_back(ourKing.upLeftOne());
						isPawnAttack = true;
					}
					if (!ourKing.isRightFile() && board.isPawn(ourKing.upRightOne())) {
						// Pawn is attacking our King
						pair.push_back(ourKing.upRightOne());
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
						pair.push_back(ourKing.downLeftOne());
						isPawnAttack = true;
					}
					if (!ourKing.isRightFile() && board.isPawn(ourKing.downRightOne())) {
						// Pawn is attacking our King
						pair.push_back(ourKing.downRightOne());
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

