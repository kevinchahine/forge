#include "MoveGenerator2.h"

using namespace std;

namespace forge
{
	void MoveGenerator2::reset()
	{
		(*this) = MoveGenerator2{};
	}

	void MoveGenerator2::preprocess(const Position & position)
	{
		bool isWhitesTurn = position.moveCounter().isWhitesTurn();
		const Board & b = position.board();

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

		ourRays = ours & b.rays();
		theirRays = theirs & b.rays();

		ourBlockers = ours & b.blockers();
		theirBlockers = theirs & b.blockers();
		
		// TODO: Don't think we need these
		ourDiagonals = ours & b.diagonals();
		theirDiagonals = theirs & b.diagonals();

		// TODO: Don't think we need these
		ourLaterals = ours & b.laterals();
		theirLaterals = theirs & b.laterals();

		// TODO: Fill this in
		ourThreats;
		theirThreats;
	}

	MoveList & MoveGenerator2::generate(const Position & pos)
	{
		reset();

		preprocess(pos);

		genPinMoves(pos.board(), pos.moveCounter().isWhitesTurn());

		return legalMoves;
	}

	void MoveGenerator2::genPinMoves(const Board & b, bool isWhitesTurn)
	{
		// 1.) --- Starting from our King, search in each ray direction for pins ---

		// --- Forward Checking ---
		// Is there a possibility of a pin in some direction?
		// If not, then theirs no reason to perform an expensive pin search in that direction
		// Is a pin possible in a diagonal direction?
		if (isPinPossible<directions::Diagonal>()) {
			// Yes, A diagonal pin is possible.
			cout << "Diagonal pin is possible\n";
			
			// --- Break it down further into individual ray directions ---
			Pin pin0 = pinSearch<directions::UR>();
			if (pin0.isValid()) { genPinMovesFor<directions::UR>(pin0); }

			Pin pin1 = pinSearch<directions::UL>();
			if (pin1.isValid()) { genPinMovesFor<directions::UL>(pin1); }

			Pin pin2 = pinSearch<directions::DL>();
			Pin pin3 = pinSearch<directions::DR>();
		}
		else {
			cout << "Diagonal pin NOT possible\n";
		}
		
		// Is a pin possible in a lateral direction?
		if (isPinPossible<directions::Lateral>()) {
			// Yes, A pin is possible.
			cout << "Lateral pin is possible\n";
		
			// --- Break it down further into individual ray directions ---
		
			Pin pin0 = pinSearch<directions::Up>();
			Pin pin1 = pinSearch<directions::Down>();
			Pin pin2 = pinSearch<directions::Left>();
			Pin pin3 = pinSearch<directions::Right>();
		}
		else {
			cout << "Lateral pin NOT possible\n";
		}

	}
} // namespace forge