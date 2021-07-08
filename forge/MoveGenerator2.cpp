#include "MoveGenerator2.h"
#include "MoveGeneratorHelpers.h"

using namespace std;

namespace forge
{
	void MoveGenerator2::reset()
	{
		(*this) = MoveGenerator2{};
	}

	void MoveGenerator2::preprocess(const Position & position)
	{
		currPositionPtr = &position;
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

		threats = MoveGenHelpers::genThreats(b, theirs);
	}

	MoveList & MoveGenerator2::generate(const Position & pos)
	{
		reset();

		preprocess(pos);

		// Who if any are attacking our King?
		AttackerPair attackers = MoveGenHelpers::findKingAttackers(pos.board(), ourKing, theirs, ours);

		// How many King attackers did we find?
		if (attackers.size() <= 2) {
			// 2 enemies are attacking our King
			// All we can do is:
			//	- King pushes to safety
			//	- King captures one attacker (assuming its not defended)

			// --- King Pushes to Safety ---
			// --- King captures one attacker ---
			genKingMoves();
		}

		if (attackers.size() <= 1) {
			// 1 enemey is attacking our King
			// All we can do is:
			//	- See attackers.size() <= 2
			//	- Non-King blocks attacker ***
			//	- Non-King captures attacker ***
			// *** Non-King pieces that are pinned to King can only move between pinner and our King
		}

		if (attackers.size() == 0) {
			// Our King is safe from attackers
			// We can do any move:
			//	- Move Absolutely Pinned Pieces
			//	- Move non-Pinned pieces
		}

		genPinMoves(pos.board(), pos.moveCounter().isWhitesTurn());	// TODO: Where should this go

		return legalMoves;
	}

	void MoveGenerator2::genPinMoves(const Board & b, bool isWhitesTurn)
	{
		// 1.) --- Starting from our King, search in each ray direction for pins ---

		// --- Forward Checking ---
		// Is there a possibility of a pin in some direction?
		// If not, then there's no reason to perform an expensive pin search in that direction
		// Is a pin possible in a diagonal direction?
		if (isPinPossible<directions::Diagonal>()) {
			// Yes, A diagonal pin is possible.
			cout << "Diagonal pin is possible\n";

			// --- Break it down further into individual ray directions ---
			searchAndGeneratePins<directions::UR>();
			searchAndGeneratePins<directions::UL>();
			searchAndGeneratePins<directions::DL>();
			searchAndGeneratePins<directions::DR>();
		}
		else {
			cout << "Diagonal pin NOT possible\n";
		}

		// Is a pin possible in a lateral direction?
		if (isPinPossible<directions::Lateral>()) {
			// Yes, A pin is possible.
			cout << "Lateral pin is possible\n";

			// --- Break it down further into individual ray directions ---
			searchAndGeneratePins<directions::Up>();
			searchAndGeneratePins<directions::Down>();
			searchAndGeneratePins<directions::Left>();
			searchAndGeneratePins<directions::Right>();
		}
		else {
			cout << "Lateral pin NOT possible\n";
		}
	}

	template<typename DIRECTION_T>
	void moveKing(
		MoveList & legals,
		const BoardSquare & ourKing,
		const BitBoard & threats,
		const BitBoard & ours,
		const Position & pos) {

		if (DIRECTION_T::wouldBeInBounds(ourKing)) {
			BoardSquare to = DIRECTION_T::move(ourKing);

			// Is move safe?
			if (threats[to] == false) {
				// Yes it is safe.

				// We can't on top of our own pieces. Is 'to' empty or their piece
				if (ours[to] == 0) {
					legals.emplace_back<pieces::King>(Move{ ourKing, to }, pos);
				}
			}
		}
	}

	void MoveGenerator2::genKingMoves()
	{
		const Position & pos = *currPositionPtr;
		const Board & board = pos.board();

		// --- Up ---
		if (!ourKing.isTopRank()) {
			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::UL>(legalMoves, ourKing, threats, ours, pos);
			}

			// --- Middle ---
			{
				moveKing<directions::Up>(legalMoves, ourKing, threats, ours, pos);
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::UR>(legalMoves, ourKing, threats, ours, pos);
			}
		}

		// --- Middle ---
		{
			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::Left>(legalMoves, ourKing, threats, ours, pos);
			}

			// --- Middle ---
			{
				// King stays where it is. Nothing to do here.
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::Right>(legalMoves, ourKing, threats, ours, pos);
			}
		}

		// --- Bottom ---
		if (!ourKing.isBotRank()) {

			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::DL>(legalMoves, ourKing, threats, ours, pos);
			}

			// --- Middle ---
			{
				moveKing<directions::Down>(legalMoves, ourKing, threats, ours, pos);
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::DR>(legalMoves, ourKing, threats, ours, pos);
			}
		}
	}
} // namespace forge