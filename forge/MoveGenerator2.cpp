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
		currPositionPtr = &position;
		bool isWhitesTurn = position.moveCounter().isWhitesTurn();
		const Board & b = position.board();

		occupied = b.occupied();
		empty = b.empty();

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

		threats = Threats::genThreats(b, theirs);
	}

	MoveList & MoveGenerator2::generate(const Position & pos)
	{
		reset();

		preprocess(pos);

		// Who if any are attacking our King?
		KingAttackers attackers = KingAttackers::findKingAttackers(pos.board(), ourKing, theirs, ours);

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

		if (attackers.size() == 1) {
			// 1 enemey is attacking our King
			// All we can do is:
			//	- See attackers.size() <= 2
			//	- Non-King blocks attacker ***
			//	- Non-King captures attacker ***
			// *** Non-King pieces that are pinned to King can only move between pinner and our King

			// TODO: Find all pinned pieces here
			genPinMoves(pos.board(), pos.moveCounter().isWhitesTurn(), true);

			// Must evaluate pinned peices before calling this method
			genBlockAndCaptureMoves(attackers[0]);
		}

		if (attackers.size() == 0) {
			// Our King is safe from attackers
			// We can do any move:
			//	- Move Absolutely Pinned Pieces
			//	- Move non-Pinned pieces

			genPinMoves(pos.board(), pos.moveCounter().isWhitesTurn(), false);

			// genFreeMoves()
		}

		return legalMoves;
	}

	void MoveGenerator2::genPinMoves(const Board & b, bool isWhitesTurn, bool searchOnly)
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
			searchAndGeneratePins<directions::UR>(searchOnly);
			searchAndGeneratePins<directions::UL>(searchOnly);
			searchAndGeneratePins<directions::DL>(searchOnly);
			searchAndGeneratePins<directions::DR>(searchOnly);
		}
		else {
			cout << "Diagonal pin NOT possible\n";
		}

		// Is a pin possible in a lateral direction?
		if (isPinPossible<directions::Lateral>()) {
			// Yes, A pin is possible.
			cout << "Lateral pin is possible\n";

			// --- Break it down further into individual ray directions ---
			searchAndGeneratePins<directions::Up>(searchOnly);
			searchAndGeneratePins<directions::Down>(searchOnly);
			searchAndGeneratePins<directions::Left>(searchOnly);
			searchAndGeneratePins<directions::Right>(searchOnly);
		}
		else {
			cout << "Lateral pin NOT possible\n";
		}
	}

	// !!!WARNING: Make sure move is in bounds before calling this function.
	// Does not perform bounds checking.
	template<typename DIRECTION_T>
	inline void moveKing(
		MoveList & legals,
		const BoardSquare & ourKing,
		const BitBoard & open,	// Every square that is not occupied and not attacked
		const Position & pos)
	{
#ifdef _DEBUG
		if (DIRECTION_T::wouldBeInBounds(ourKing) == false) {
			cout << guten::color::push() << guten::color::red
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " move is out of bounds.\n"
				<< guten::color::pop();
			return;
		}
#endif // _DEBUG

		BoardSquare to = DIRECTION_T::move(ourKing);

		// Is move safe?
		if (open[to]) {
			// Yes it is safe.
			legals.emplace_back<pieces::King>(Move{ ourKing, to }, pos);
		}
	}

	void MoveGenerator2::genKingMoves()
	{
		const Position & pos = *currPositionPtr;
		const Board & board = pos.board();
		const BitBoard open = ~(ours | threats);	// Squares that are not ours and not attacked

		// --- Up ---
		if (!ourKing.isTopRank()) {
			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::UL>(legalMoves, ourKing, open, pos);
			}

			// --- Middle ---
			{
				moveKing<directions::Up>(legalMoves, ourKing, open, pos);
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::UR>(legalMoves, ourKing, open, pos);
			}
		}

		// --- Middle ---
		{
			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::Left>(legalMoves, ourKing, open, pos);
			}

			// --- Middle ---
			{
				// King stays where it is. Nothing to do here.
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::Right>(legalMoves, ourKing, open, pos);
			}
		}

		// --- Bottom ---
		if (!ourKing.isBotRank()) {

			// --- Left ---
			if (!ourKing.isLeftFile()) {
				moveKing<directions::DL>(legalMoves, ourKing, open, pos);
			}

			// --- Middle ---
			{
				moveKing<directions::Down>(legalMoves, ourKing, open, pos);
			}

			// --- Right ---
			if (!ourKing.isRightFile()) {
				moveKing<directions::DR>(legalMoves, ourKing, open, pos);
			}
		}
	}

	template<typename RAY_DIRECTION_T>
	inline void captureAttackerWithRay(
		MoveList & legals,
		const BoardSquare & attacker,
		const BitBoard & theirs,
		const BitBoard & ours,
		const Position & pos)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"Error: This function only works on Ray directions.");

		BoardSquare bs = Attackers::findAttackingRay<RAY_DIRECTION_T>(attacker, pos.board(), theirs, ours);

		// If an attacker was found and it is not Absolutely Pinned,
		// Apply capture move.
		if (bs.isValid()) {
			// TODO: What about promotions.
			legals.emplace_back<pieces::Piece>(Move{ bs, attacker }, pos);
		}
	}

	template<typename KNIGHT_DIRECTION_T>
	inline void captureAttackerWithKnight(
		MoveList & legals,
		const BoardSquare & attacker,
		const Position & pos,
		const BitBoard & ourKnights)
	{
		static_assert(std::is_base_of<directions::LShape, KNIGHT_DIRECTION_T>(),
			"Error: This function only works on Knight directions.");
		
		BoardSquare bs = Attackers::findAttackingKnight<KNIGHT_DIRECTION_T>(attacker, pos.board(), ourKnights);

		// If an attacker was found and it is not Absolutely Pinned,
		// Apply capture move.
		if (bs.isValid()) {
			legals.emplace_back<pieces::Knight>(Move{ bs, attacker }, pos);
		}
	}

	void MoveGenerator2::genBlockAndCaptureMoves(const KingAttacker & attacker)
	{
		const Board & board = currPositionPtr->board();
		pieces::Piece attackingPiece = board.at(attacker.square);

		BitBoard pushMask;		// Where the attacker can push to (assuming no obstacles)
		BitBoard captureMask;	// Where the attacker can capture (assuming each square can be captured)

		// Generate push and capture masks that tell us where attacker can push and capture to
		attackingPiece.masks(attacker.square, pushMask, captureMask);

		// Is the attacker a Ray or non-Ray?

		if (attackingPiece.isRay()) {
			// Yes. Because the attacker is Ray, it can be blocked.

			// TODO: Optimize: There is no need to search for blockers and capturers
			// in direction of King because, those pieces would be taken care of as 
			// pins.

			const directions::Direction & dir = attacker.dir;

			BoardSquare it = ourKing;
			while (dir.wouldBeInBounds(it)) {
				it = dir.move(it);	// Incremented first to skip kings coordinate

				// --- Find one of our pieces that can block/capture the attacker ---

				// Find a piece that can move to this square and block the attacker.

				// --- Our Laterals ---
				///findAndMoveBlockers<directions::Up>(it);
				///findAndMoveBlockers<directions::Down>(it);
				///findAndMoveBlockers<directions::Left>(it);
				///findAndMoveBlockers<directions::Right>(it);

				// --- Our Diagonals ---
				///findAndMoveBlockers<directions::UL>(it);
				///findAndMoveBlockers<directions::UR>(it);
				///findAndMoveBlockers<directions::DL>(it);
				///findAndMoveBlockers<directions::DR>(it);

				// --- Our Knights ---


				// --- Our Pawns ---

				// --- Our Kings (Can't block attacks) ---
				// Excluded because King can't block himself. Nothing to do here
			}
		}

		// --- See if we can Capture the Attacker ---

		// Attacker must be either a Pawn or Knight.
		// Since we can't block it, lets see if we can capture it.

		// --- Look for Lateral Captures ---
		{
			const BoardSquare & as = attacker.square;
			BitBoard attackerCross = BitBoard::mask<directions::Lateral>(as);
			BitBoard possibleCaptures = ourLaterals & attackerCross & ~ourAbsolutePins;

			// Is it possible that one of our Laterals can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. Look in more detail.

				captureAttackerWithRay<directions::Up>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::Down>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::Left>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::Right>(legalMoves, as, theirs, ours, *currPositionPtr);
			}
		}

		// --- Look for Diagonal Captures ---
		{
			const BoardSquare & as = attacker.square;
			BitBoard attackerX = BitBoard::mask<directions::Diagonal>(as);
			BitBoard possibleCaptures = ourDiagonals & attackerX & ~ourAbsolutePins;

			// Is it possible that one of our Laterals can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. Look in more detail.

				captureAttackerWithRay<directions::UR>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::UL>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::DR>(legalMoves, as, theirs, ours, *currPositionPtr);
				captureAttackerWithRay<directions::DL>(legalMoves, as, theirs, ours, *currPositionPtr);
			}
		}

		// --- Look for Knight Captures ---
		{
			const BoardSquare & as = attacker.square;
			BitBoard attackerOctopus = BitBoard::mask<directions::LShape>(as);
			BitBoard possibleCaptures = board.knights() & ours & attackerOctopus & ~ourAbsolutePins;

			// Is it possible that one of our Knights can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. One or more of our Knights can capture attacker.

				captureAttackerWithKnight<directions::Knight0>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight1>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight2>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight3>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight4>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight5>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight6>(legalMoves, as, *currPositionPtr, possibleCaptures);
				captureAttackerWithKnight<directions::Knight7>(legalMoves, as, *currPositionPtr, possibleCaptures);
			}
		}

		// --- Look for Pawn Captures ---
		{
			const BoardSquare & as = attacker.square;
			BitBoard pawnCapturers = ours & board.pawns() & ~ourAbsolutePins;

			if (attackingPiece.isWhite()) {
				// Attacking piece is White. Lets see if we can capture it with a Black Pawn.
				if (as.isTopRank() == false) {
					// --- Look Up Left ---
					if (as.isLeftFile() == false) {
						BoardSquare pawn = as.upLeftOne();

						if (pawnCapturers[pawn]) {
							if (as.isBotRank()) {
								// Capture will result in promotion
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackQueen }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackRook }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackBishop }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackKnight }, *currPositionPtr);
							}
							else {
								// Capture will NOT result in promotion
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, }, *currPositionPtr);
							}
						}
					}

					// --- Look Up Right ---
					if (as.isRightFile() == false) {
						BoardSquare pawn = as.upRightOne();

						if (pawnCapturers[pawn]) {
							if (as.isBotRank()) {
								// Capture will result in promotion
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackQueen }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackRook }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackBishop }, *currPositionPtr);
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, pieces::blackKnight }, *currPositionPtr);
							}
							else {
								// Capture will NOT result in promotion
								legalMoves.emplace_back<pieces::BlackPawn>(Move{ pawn, as, }, *currPositionPtr);
							}
						}
					}
				}
			}
			else {
				// Attacking piece is Black. Lets see if we can capture it with a White Pawn.
				if (as.isBotRank() == false) {
					// --- Look Down Left ---
					if (as.isLeftFile() == false) {
						BoardSquare pawn = as.downLeftOne();

						if (pawnCapturers[pawn]) {
							if (as.isTopRank()) {
								// Capture will result in promotion
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteQueen }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteRook }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteBishop }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteKnight }, *currPositionPtr);
							}
							else {
								// Capture will NOT result in promotion
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, }, *currPositionPtr);
							}
						}
					}

					// --- Look Down Right ---
					if (as.isRightFile() == false) {
						BoardSquare pawn = as.downRightOne();

						if (pawnCapturers[pawn]) {
							if (as.isTopRank()) {
								// Capture will result in promotion
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteQueen }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteRook }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteBishop }, *currPositionPtr);
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, pieces::whiteKnight }, *currPositionPtr);
							}
							else {
								// Capture will NOT result in promotion
								legalMoves.emplace_back<pieces::WhitePawn>(Move{ pawn, as, }, *currPositionPtr);
							}
						}
					}
				}
			}
		}

		// --- Exclude King Captures (Those are taken care of in genKingMoves() ---
	}
} // namespace forge