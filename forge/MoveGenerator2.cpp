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

		// TODO: Fill this in
		ourThreats;
		///theirThreats = MoveGenHelpers::genThreats(*boardPtr, ours);
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
} // namespace forge