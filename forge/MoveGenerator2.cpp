#include "MoveGenerator2.h"

using namespace std;

namespace forge
{
	void MoveGenerator2::reset()
	{
		(*this) = MoveGenerator2{};
	}

	void MoveGenerator2::preprocess(const Board & b, bool isWhitesTurn)
	{
		if (isWhitesTurn) {
			ours = b.whites();
			theirs = b.blacks();

			ourKing = b.whiteKing();
			theirKing = b.blackKing();
		}
		else {
			ours = b.blacks();
			theirs = b.whites();

			ourKing = b.blackKing();
			theirKing = b.whiteKing();
		}

		ourRays = b.rays() & ours;
		theirRays = b.rays() & theirs;

		ourBlockers = ours & ~b.kings();
		theirBlockers = theirs & ~b.kings();
		
		ourDiagonals = ours & b.diagonals();
		theirDiagonals = theirs & b.diagonals();

		ourLaterals = ours & b.laterals();
		theirLaterals = theirs & b.laterals();

		// TODO: Fill this in
		ourThreats;
		theirThreats;
	}

	MoveList & MoveGenerator2::generate(const Position & pos)
	{
		reset();


		return legalMoves;
	}

	void MoveGenerator2::genPinMoves(const Board & b, bool isWhitesTurn)
	{
		// 1.) --- Starting from our King, search in each ray direction for pins ---

		// --- Forward Checking ---
		// Is there a possibility of a pin in some direction?
		// If not, then theirs no reason to perform an expensive pin search in that direction

		// Is a pin possible in a diagonal direction?
		if (isPinPossible<directions::Diagonal>(theirDiagonals, BitBoard(0xffff'ffff'ffff'ffff))) {
			// Yes, A pin is possible.
		
			// --- Break it down further into individual ray directions ---
		
			if (isPinPossible<directions::UR>(theirDiagonals, ourBlockers)) {}
			if (isPinPossible<directions::UL>(theirDiagonals, ourBlockers)) {}
			if (isPinPossible<directions::DL>(theirDiagonals, ourBlockers)) {}
			if (isPinPossible<directions::DR>(theirDiagonals, ourBlockers)) {}
		}
		
		// Is a pin possible in a lateral direction?
		if (isPinPossible<directions::Lateral>(theirLaterals, BitBoard(0xffff'ffff'ffff'ffff))) {
			// Yes, A pin is possible.
		
			// --- Break it down further into individual ray directions ---
		
			if (isPinPossible<directions::Up>(theirLaterals, ourBlockers)) {}
			if (isPinPossible<directions::Down>(theirLaterals, ourBlockers)) {}
			if (isPinPossible<directions::Left>(theirLaterals, ourBlockers)) {}
			if (isPinPossible<directions::Right>(theirLaterals, ourBlockers)) {}
		}
	}
} // namespace forge