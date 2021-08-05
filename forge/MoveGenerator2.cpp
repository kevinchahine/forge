#include "MoveGenerator2.h"

using namespace std;

namespace forge
{
	void MoveGenerator2::reset()
	{
		(*this) = MoveGenerator2{};
	}

	void MoveGenerator2::preprocess(const Position& position)
	{
		currPositionPtr = &position;
		bool isWhitesTurn = position.moveCounter().isWhitesTurn();
		const Board& b = position.board();

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

	MoveList& MoveGenerator2::generate(const Position& pos)
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

			// TODO: OPTIMIZE: genKingMoves() should be called last.
			// When sorting moves in order of best to worst, King moves will usually be last.
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

			genFreeMoves();
		}

		return legalMoves;
	}

	void MoveGenerator2::genPinMoves(const Board& b, bool isWhitesTurn, bool searchOnly)
	{
		// 1.) --- Starting from our King, search in each ray direction for pins ---
		// Remember: Pins can only be delivered from Ray Pieces (Not knights, kings or pawns)

		// --- Forward Checking ---
		// Is there a possibility of a pin in some direction?
		// If not, then there's no reason to perform an expensive pin search in that direction
		// Is a pin possible in a diagonal direction?
		if (isPinPossible<directions::Diagonal>()) {
			// Yes, A diagonal pin is possible.
			///cout << "Diagonal pin is possible\n";

			// --- Break bs down further into individual ray directions ---
			searchAndGeneratePins<directions::UR>(searchOnly);
			searchAndGeneratePins<directions::UL>(searchOnly);
			searchAndGeneratePins<directions::DL>(searchOnly);
			searchAndGeneratePins<directions::DR>(searchOnly);
		}
		else {
			///cout << "Diagonal pin NOT possible\n";
		}

		// Is a pin possible in a lateral direction?
		if (isPinPossible<directions::Lateral>()) {
			// Yes, A pin is possible.
			///cout << "Lateral pin is possible\n";

			// --- Break bs down further into individual ray directions ---
			searchAndGeneratePins<directions::Up>(searchOnly);
			searchAndGeneratePins<directions::Down>(searchOnly);
			searchAndGeneratePins<directions::Left>(searchOnly);
			searchAndGeneratePins<directions::Right>(searchOnly);
		}
		else {
			///cout << "Lateral pin NOT possible\n";
		}
	}

	// !!!WARNING: Make sure move is in bounds before calling this function.
	// Does not perform bounds checking.
	template<typename DIRECTION_T>
	inline void moveKing(
		MoveList& legals,
		const BoardSquare& ourKing,
		const BitBoard& open,	// Every square that is not occupied and not attacked
		const Position& pos)
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
			// Yes bs is safe.
			legals.emplace_back<pieces::King>(Move{ ourKing, to }, pos);
		}
	}

	void MoveGenerator2::genKingMoves()
	{
		const Position& pos = *currPositionPtr;
		const Board& board = pos.board();
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
				// King stays where bs is. Nothing to do here.
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
		MoveList& legals,
		const BoardSquare& attacker,
		const BitBoard& theirs,
		const BitBoard& ours,
		const Position& pos)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"Error: This function only works on Ray directions.");

		BoardSquare bs = Attackers::findAttackingRay<RAY_DIRECTION_T>(attacker, pos.board(), theirs, ours);

		// If an attacker was found and bs is not Absolutely Pinned,
		// Apply capture move.
		if (bs.isValid()) {
			// TODO: OPTIMIZE: Replace template param with pieces::RayPiece
			legals.emplace_back<pieces::Piece>(Move{ bs, attacker }, pos);
		}
	}

	template<typename KNIGHT_DIRECTION_T>
	inline void captureAttackerWithKnight(
		MoveList& legals,
		const BoardSquare& attacker,
		const Position& pos,
		const BitBoard& ourKnights)
	{
		static_assert(std::is_base_of<directions::LShape, KNIGHT_DIRECTION_T>(),
			"Error: This function only works on Knight directions.");

		BoardSquare bs = Attackers::findAttackingKnight<KNIGHT_DIRECTION_T>(attacker, pos.board(), ourKnights);

		// If an attacker was found and bs is not Absolutely Pinned,
		// Apply capture move.
		if (bs.isValid()) {
			legals.emplace_back<pieces::Knight>(Move{ bs, attacker }, pos);
		}
	}

	void MoveGenerator2::genBlockAndCaptureMoves(const KingAttacker& attacker)
	{
		const Position& pos = *currPositionPtr;
		const Board& board = pos.board();
		BitBoard pushMask;		// Where the attacker can push to (assuming no obstacles)
		BitBoard captureMask;	// Where the attacker can capture (assuming each square can be captured)

		// Generate push and capture masks that tell us where attacker can push and capture to
		const pieces::Piece& attackingPiece = board.at(attacker.square);
		attackingPiece.masks(attacker.square, pushMask, captureMask);

		// Is the attacker a Ray or non-Ray?
		if (attackingPiece.isRay()) {
			// Yes. Because the attacker is Ray, bs can be blocked.
			// Lets find a piece that can block or capture the attacker. 

			// TODO: Optimize: There is no need to search for blockers and capturers
			// in direction of King because, those pieces would be taken care of as 
			// pins.

			const directions::Direction& dir = attacker.dir;

			// Iterate from King's square to attacker and look for blocker/captures
			BoardSquare bs = ourKing;
			while (dir.wouldBeInBounds(bs)) {
				// TODO: Optimize: We can probably replace bounds checking with attacker.square
				// 'bs' will always reach attacker before edge of board.

				// TODO: Optimize: We should probably have multiple loops. One loop for each type of piece.

				bs = dir.move(bs);	// Incremented first to skip kings coordinate

				// --- Find one of our pieces that can block/capture the attacker ---

				// Find a piece that can move to this square and block the attacker.
				// Hint: If a piece can 'attack' a square then bs can block/capture at that square.

				// --- Look for Lateral Captures ---
				{
					BitBoard attackerCross = BitBoard::mask<directions::Lateral>(bs);
					BitBoard possibleCaptures = ourLaterals & attackerCross & ~ourAbsolutePins;

					// Is bs possible that one of our Laterals can capture attacker?
					if (possibleCaptures.any()) {
						// Yes. Look in more detail.

						captureAttackerWithRay<directions::Up>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::Down>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::Left>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::Right>(legalMoves, bs, ours, theirs, pos);
					}
				}

				// --- Our Diagonals ---
				{
					BitBoard attackerX = BitBoard::mask<directions::Diagonal>(bs);
					BitBoard possibleCaptures = ourDiagonals & attackerX & ~ourAbsolutePins;

					// Is bs possible that one of our Laterals can capture attacker?
					if (possibleCaptures.any()) {
						// Yes. Look in more detail.

						captureAttackerWithRay<directions::UR>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::UL>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::DR>(legalMoves, bs, ours, theirs, pos);
						captureAttackerWithRay<directions::DL>(legalMoves, bs, ours, theirs, pos);
					}
				}

				// --- Our Knights ---
				{
					BitBoard attackerOctopus = BitBoard::mask<directions::LShape>(bs);
					BitBoard possibleCaptures = board.knights() & ours & attackerOctopus & ~ourAbsolutePins;

					// Is bs possible that one of our Knights can capture attacker?
					if (possibleCaptures.any()) {
						// Yes. One or more of our Knights can capture attacker.

						captureAttackerWithKnight<directions::Knight0>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight1>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight2>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight3>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight4>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight5>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight6>(legalMoves, bs, pos, possibleCaptures);
						captureAttackerWithKnight<directions::Knight7>(legalMoves, bs, pos, possibleCaptures);
					}
				}

				// --- Our Pawns ---


				// --- Our Kings (Can't block attacks) ---
				// Excluded because King can't block himself. Nothing to do here

				// Did we reach the attacker?
				if (bs == attacker.square) {
					break; // Yes we can now break from loop and move on to captures
				}
			}
		}

		// --- See if we can Capture the Attacker ---

		// Attacker must be either a Pawn or Knight.
		// Since we can't block bs, lets see if we can capture bs.
		
		// --- Look for Captures from our Lateral Pieces ---
		{
			const BoardSquare& as = attacker.square;
			BitBoard attackerCross = BitBoard::mask<directions::Lateral>(as);
			BitBoard possibleCaptures = ourLaterals & attackerCross & ~ourAbsolutePins;

			// Is bs possible that one of our Laterals can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. Look in more detail.

				captureAttackerWithRay<directions::Up>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::Down>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::Left>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::Right>(legalMoves, as, theirs, ours, pos);
			}
		}
		
		// --- Look for Captures from our Diagonal Pieces ---
		{
			const BoardSquare& as = attacker.square;
			BitBoard attackerX = BitBoard::mask<directions::Diagonal>(as);
			BitBoard possibleCaptures = ourDiagonals & attackerX & ~ourAbsolutePins;

			// Is bs possible that one of our Diagonals can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. Look in more detail.

				captureAttackerWithRay<directions::UR>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::UL>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::DR>(legalMoves, as, theirs, ours, pos);
				captureAttackerWithRay<directions::DL>(legalMoves, as, theirs, ours, pos);
			}
		}

		// --- Look for Captures from our Knights ---
		{
			const BoardSquare& as = attacker.square;
			BitBoard attackerOctopus = BitBoard::mask<directions::LShape>(as);
			BitBoard possibleCaptures = board.knights() & ours & attackerOctopus & ~ourAbsolutePins;

			// Is bs possible that one of our Knights can capture attacker?
			if (possibleCaptures.any()) {
				// Yes. One or more of our Knights can capture attacker.

				captureAttackerWithKnight<directions::Knight0>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight1>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight2>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight3>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight4>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight5>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight6>(legalMoves, as, pos, ours);
				captureAttackerWithKnight<directions::Knight7>(legalMoves, as, pos, ours);
			}
		}

		// --- Look for Captures from our Pawns ---
		genPawnBlockAndCaptureRay(attacker, captureMask);

		// --- Exclude Captures from Kings (Those are taken care of in genKingMoves() ---
	}

	void MoveGenerator2::genPawnBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask)
	{
		const Position& pos = *currPositionPtr;
		const Board& board = pos.board();
		pieces::Piece attackerPiece = board.at(attacker.square);
		const directions::Direction& direction = attacker.dir;

#ifdef _DEBUG
		if (attackerPiece.isRay() == false) {
			cout << "Error: " << __FUNCTION__ << "() is only ment to be called when attacker is a Ray"
				<< endl;
		}
#endif // _DEBUG

		// Our pawns that might be able to block/capture attacker.
		BitBoard usefullPawns = ours & board.pawns() & ~ourAbsolutePins;

		directions::Direction dir1;	// Direction of blocking pawn that pushes 1 squares
		directions::Direction dir2;	// Direction of blocking pawn that pushes 2 squares
		directions::Direction dirL;	// Direction of capturing pawn that captures from left
		directions::Direction dirR;	// Direction of capturing pawn that captures from right
		uint8_t promotionRow;		// Promotional rank of pawn
		uint8_t startingRow;		// Row a pawn starts on (and can push 2 squares)
		pieces::Piece q;
		pieces::Piece r;
		pieces::Piece b;
		pieces::Piece n;

		// What is the color of Attacker? 
		if (attackerPiece.isWhite()) {
			// === Block/Capture with Black Pawn ===
			dir1 = directions::Up{};
			dir2 = directions::Direction{ -2, 0 };
			dirL = directions::UL{};
			dirR = directions::UR{};
			promotionRow = 7;
			startingRow = 1;

			q = pieces::blackQueen;
			r = pieces::blackRook;
			b = pieces::blackBishop;
			n = pieces::blackKnight;
		}
		else {
			// === Block/Capture with White Pawn ===
			dir1 = directions::Down{};
			dir2 = directions::Direction{ 2, 0 };
			dirL = directions::DL{};
			dirR = directions::DR{};
			promotionRow = 0;
			startingRow = 6;

			q = pieces::whiteQueen;
			r = pieces::whiteRook;
			b = pieces::whiteBishop;
			n = pieces::whiteKnight;
		}

		// TODO: Optimize: The commented part could save a lot of work
		if (/*direction.isHorizontal() || direction.isDiagonal()*/ true) {
			// Only need to search pawn pushes. No need to look for pawn captures.
			// Iterate from king in the direction of attacker 
			// And see if we can block the attacker.
			for (BoardSquare square = direction.move(ourKing); 
				square != attacker.square; 
				square = direction.move(square)) 
			{
				// === PUSH 1 ===
				BoardSquare pawn1 = (dir1.wouldBeInBounds(square) ? dir1.move(square) : BoardSquare::invalid());

				// Hint: square will always be empty because if it wasn't then an attack would not be possible
				if (pawn1.isValid() && usefullPawns[pawn1]/* && empty[square]*/) {
					// Would push lead to promotion?
					if (pawn1.row() == promotionRow) {
						// Yes promote pawn.
						legalMoves.emplace_back<pieces::Pawn>(Move{ pawn1, square, q }, pos);
						legalMoves.emplace_back<pieces::Pawn>(Move{ pawn1, square, r }, pos);
						legalMoves.emplace_back<pieces::Pawn>(Move{ pawn1, square, b }, pos);
						legalMoves.emplace_back<pieces::Pawn>(Move{ pawn1, square, n }, pos);
					}
					else {
						// No Promotion. Just a push.
						legalMoves.emplace_back<pieces::Pawn>(Move{ pawn1, square }, pos);
					}
				}

				// === PUSH 2 ===
				BoardSquare pawn2 = (dir2.wouldBeInBounds(square) ? dir2.move(square) : BoardSquare::invalid());
				pawn2 = (
					pawn2.isValid() &&				// Make sure pawn2 is on the board
					empty[pawn1] &&					// Make sure pawn1 is empty (so that we don't jump another piece)
					usefullPawns[pawn2] &&			// Make sure pawn2 is the square of a pawn we can use and not something else
					pawn2.row() == startingRow ?	// Make sure pawn2 is a starting square where pawns can do double pushes
					pawn2 :							// True: leave pawn2 with the same value
					BoardSquare::invalid());		// False: make pawn2 invalid.

				if (pawn2.isValid()) {
					legalMoves.emplace_back<pieces::Pawn>(Move{ pawn2, square }, pos);
				}
			} // for (
		} // if Horizontal or Diagonal
		else if (direction.isVertical()) {
			// No need to search pawn pushes. Only look for pawn captures.
		}
		else {
			// This line should never be reached
		}

		// --- Look for a Pawn that can CAPTURE attacker ---

		// --- LEFT Pawn Capture ---
		BoardSquare pawnL = (dirL.wouldBeInBounds(attacker.square) ? dirL.move(attacker.square) : BoardSquare::invalid());
		if (pawnL.isValid() && usefullPawns[pawnL]) {
			if (pawnL.row() == promotionRow) {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnL, attacker.square, q }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnL, attacker.square, r }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnL, attacker.square, b }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnL, attacker.square, n }, pos);
			}
			else {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnL, attacker.square }, pos);
			}
		}

		// --- RIGHT Pawn Capture ---
		BoardSquare pawnR = (dirR.wouldBeInBounds(attacker.square) ? dirR.move(attacker.square) : BoardSquare::invalid());
		if (pawnR.isValid() && usefullPawns[pawnR]) {
			if (pawnR.row() == promotionRow) {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnR, attacker.square, q }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnR, attacker.square, r }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnR, attacker.square, b }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnR, attacker.square, n }, pos);
			}
			else {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawnR, attacker.square }, pos);
			}
		}
	}

	void MoveGenerator2::genLateralBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask)
	{

	}

	void MoveGenerator2::genDiagonalBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask)
	{

	}

	void MoveGenerator2::genKnightBlockAndCaptureRay(const KingAttacker& attacker, BitBoard captureMask)
	{

	}

	// -------------------------------- FREE MOVES ----------------------------

	void MoveGenerator2::genFreeMoves()
	{
		const Board& b = currPositionPtr->board();

		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				BoardSquare cell{ row, col };

				// Is this piece pinned? If so bs's moves would have been generated by another function.
				if (ours[cell] && ourAbsolutePins[cell] == false) {
					// This piece is not pinned, and therefore its moves have not been generated.

					pieces::Piece p = b.at(cell);

					// --- In order of most common to least common pieces
					if (p.isPawn()) {
						genFreePawnMoves(cell);
					}
					else if (p.isRook()) {
						genFreeRookMoves(cell);
					}
					else if (p.isBishop()) {
						genFreeBishopMoves(cell);
					}
					else if (p.isKnight()) {
						genFreeKnightMoves(cell);
					}
					else if (p.isQueen()) {
						genFreeQueenMoves(cell);
					}
					/*else if (p.isKing()) {
						// King moves are generated in another method
						// Nothing to do here
					}*/
				}
			}
		}
	}

	void MoveGenerator2::genFreePawnMoves(BoardSquare pawn)
	{
		const Board& board = currPositionPtr->board();
		const Position& pos = *currPositionPtr;

#ifdef _DEBUG
		if (board.isPawn(pawn) == false) {
			cout << guten::color::push() << guten::color::lightred
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " " << pawn << " is a " << board.at(pawn) << " and not a pawn" << endl;
		}
#endif // _DEBUG

		// TODO: OPTIMIZE: This whole function can be optimized 
		// Break down into Black and White Pawns 

		BoardSquare push1;
		BoardSquare push2;
		BoardSquare captLeft;
		BoardSquare captRight;
		uint8_t promotionRow;
		pieces::Piece q;
		pieces::Piece r;
		pieces::Piece b;
		pieces::Piece n;

		if (board.isWhite(pawn)) {
			// === White Pawn ===
			push1 = pawn.upOne();
			push2 = (pawn.row() == 6 ? pawn.up(2) : BoardSquare::invalid());
			captLeft = (!pawn.isLeftFile() ? pawn.upLeftOne() : BoardSquare::invalid());
			captRight = (!pawn.isRightFile() ? pawn.upRightOne() : BoardSquare::invalid());
			promotionRow = 0;
			q = pieces::whiteQueen;
			r = pieces::whiteRook;
			b = pieces::whiteBishop;
			n = pieces::whiteKnight;
		}
		else {
			// === Black Pawn ===
			push1 = pawn.downOne();
			push2 = (pawn.row() == 1 ? pawn.down(2) : BoardSquare::invalid());
			captLeft = (!pawn.isLeftFile() ? pawn.downLeftOne() : BoardSquare::invalid());
			captRight = (!pawn.isRightFile() ? pawn.downRightOne() : BoardSquare::invalid());
			promotionRow = 7;
			q = pieces::blackQueen;
			r = pieces::blackRook;
			b = pieces::blackBishop;
			n = pieces::blackKnight;
		}

		// === PUSH 1 ===
		if (empty[push1]) {
			if (push1.row() == promotionRow) {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push1, q }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push1, r }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push1, b }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push1, n }, pos);
			}
			else {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push1 }, pos);

				// === PUSH 2 ===
				if (push2.isValid() && empty[push2]) {
					legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, push2 }, pos);
				}
			}
		}

		// === CAPTURE LEFT ===
		if (captLeft.isValid() && theirs[captLeft]) {
			if (captLeft.row() == promotionRow) {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captLeft, q }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captLeft, r }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captLeft, b }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captLeft, n }, pos);
			}
			else {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captLeft }, pos);
			}
		}

		// === CAPTURE RIGHT ===
		if (captRight.isValid() && theirs[captRight]) {
			if (captRight.row() == promotionRow) {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captRight, q }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captRight, r }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captRight, b }, pos);
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captRight, n }, pos);
			}
			else {
				legalMoves.emplace_back<pieces::Pawn>(Move{ pawn, captRight }, pos);
			}
		}

		// TODO: ENPASSENT: Don't forget enpassent
	}

	// Generates moves of a ray piece in some direction.
	// Iterates from 'ray' to edge of board or until an obstacle is hit.
	// Generates capture and push moves
	// 'ray' - Coordinate of ray piece of which moves are to be generated
	// 'obstacles - BitBoard of allToFen pieces
	// 'theirs' - BitBoard of allToFen pieces of opposite color to piece at 'ray'
	// 'legals' - MoveList of legal moves
	// 'pos' - Current position
	template<typename RAY_DIRECTION_T, typename PIECE_T>
	void genRayMoves(BoardSquare ray, BitBoard occupied, BitBoard theirs, MoveList& legals, const Position& pos)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"This function can only be called on a Ray direction");

		BoardSquare it = ray;

		while (RAY_DIRECTION_T::wouldBeInBounds(it)) {
			it = RAY_DIRECTION_T::move(it);

			if (occupied[it]) {
				// Cell is occupied either by one of our or their pieces

				if (theirs[it]) {
					// Capture Piece
					legals.emplace_back<PIECE_T>(Move{ ray, it }, pos);
				}

				// Break at every obstacle
				break;
			}
			else {
				// Cell is empty
				legals.emplace_back<PIECE_T>(Move{ ray, it }, pos);
			}
		}
	}

	void MoveGenerator2::genFreeRookMoves(BoardSquare rook)
	{
		const Position& pos = *currPositionPtr;
		const Board& b = pos.board();

#ifdef _DEBUG
		if (b.isRook(rook) == false) {
			cout << guten::color::push() << guten::color::lightred
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " " << rook << " is a " << b.at(rook) << " and not a rook" << endl;
		}
#endif // _DEBUG

		genRayMoves<directions::Up, pieces::Rook>(rook, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Down, pieces::Rook>(rook, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Left, pieces::Rook>(rook, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Right, pieces::Rook>(rook, occupied, theirs, legalMoves, pos);
	}

	template<typename KNIGHT_DIRECTION_T>
	void genKnightMoves(BoardSquare knight, BitBoard ours, MoveList& legals, const Position& pos)
	{
		if (KNIGHT_DIRECTION_T::wouldBeInBounds(knight)) {
			BoardSquare to = KNIGHT_DIRECTION_T::move(knight);

			if (ours[to] == false) {
				legals.emplace_back<pieces::Knight>(Move{ knight, to }, pos);
			}
		}
	}

	void MoveGenerator2::genFreeKnightMoves(BoardSquare knight)
	{
		const Position& pos = *currPositionPtr;
		const Board& b = pos.board();

#ifdef _DEBUG
		if (b.isKnight(knight) == false) {
			cout << guten::color::push() << guten::color::lightred
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " " << knight << " is a " << b.at(knight) << " and not a knight" << endl;
		}
#endif // _DEBUG

		// TODO: OPTIMIZE: Bounds checking is redundant here.
		// find a way to optimize bounds checking in this function.

		genKnightMoves<directions::Knight0>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight1>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight2>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight3>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight4>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight5>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight6>(knight, ours, legalMoves, pos);
		genKnightMoves<directions::Knight7>(knight, ours, legalMoves, pos);
	}

	void MoveGenerator2::genFreeBishopMoves(BoardSquare bishop)
	{
		const Position& pos = *currPositionPtr;
		const Board& b = pos.board();

#ifdef _DEBUG
		if (b.isBishop(bishop) == false) {
			cout << guten::color::push() << guten::color::lightred
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " " << bishop << " is a " << b.at(bishop) << " and not a bishop" << endl;
		}
#endif // _DEBUG

		genRayMoves<directions::UL, pieces::Bishop>(bishop, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::UR, pieces::Bishop>(bishop, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::DL, pieces::Bishop>(bishop, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::DR, pieces::Bishop>(bishop, occupied, theirs, legalMoves, pos);
	}

	void MoveGenerator2::genFreeQueenMoves(BoardSquare queen)
	{
		const Position& pos = *currPositionPtr;
		const Board& b = pos.board();

#ifdef _DEBUG
		if (b.isQueen(queen) == false) {
			cout << guten::color::push() << guten::color::lightred
				<< "Error: " << __FILE__ << " line " << __LINE__
				<< " " << queen << " is a " << b.at(queen) << " and not a pawn" << endl;
		}
#endif // _DEBUG

		genRayMoves<directions::Up, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Down, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Left, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::Right, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);

		genRayMoves<directions::UL, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::UR, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::DL, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
		genRayMoves<directions::DR, pieces::Queen>(queen, occupied, theirs, legalMoves, pos);
	}
} // namespace forge
