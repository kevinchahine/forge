#include "GameState.h"
#include "AttackChecker.h"
#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	std::ostream & forge::operator<<(std::ostream & os, const GameState & gs)
	{
		if (gs.state != GameState::STATE::DRAW) {
			switch (gs.player)
			{
			case GameState::PLAYER::WHITE:	os << "White";	break;
			case GameState::PLAYER::BLACK:	os << "Black";	break;
			}
		}

		switch (gs.state)
		{
		case GameState::STATE::CONTINUE:	os << "'s turn";	break;
		case GameState::STATE::WIN:			os << " won";		break;
		case GameState::STATE::DRAW:		os << "Draw";		break;
		}

		switch (gs.reason)
		{
		case GameState::REASON::NOTHING:												break;
		case GameState::REASON::AGREEMENT:					os << " by aggreement";		break;
		case GameState::REASON::RESIGNATION:				os << " by resignation";	break;
		case GameState::REASON::CHECKMATE:					os << " by checkmate";		break;
		case GameState::REASON::STALEMATE:					os << " by stalemate";		break;
		case GameState::REASON::REPETITION:					os << " by repetition";		break;
		case GameState::REASON::FIFTY_MOVE_RULE:			os << " by 50 Move Rule";	break;
		case GameState::REASON::INSUFFICIENT_MATERIAL_ONLY: os << " by insufficient material";	break;
		case GameState::REASON::TIMEOUT_ONLY:				os << " by timeout";		break;
		case GameState::REASON::TIMEOUT_AND_INSUFFICIENT_MATERIAL:	os << " by timeout with insufficient material";  break;
		}

		return os;
	}

	void GameState::operator()(Node * node)
	{
		// Make sure that children nodes have been generated
		if (node->isExpanded() == false) {
			node->expand();	// generate children Nodes
		}

		function<bool()> drawByRepetition = [&]() {
			return GameState::isDrawByRepetition(node);
		};

		calcGameState(
			node->children().size(),		// Number of legal moves
			node->position(),				// current position
			std::move(drawByRepetition));	// calculates draw by repetition using a Node tree
	}

	void GameState::operator()(const GameHistory & history)
	{
		// Calculate number of legal moves
		MoveList legals = MoveGenerator::generateLegalMoves(history.current());

		function<bool()> drawByRepetition = [&]() {
			return GameState::isDrawByRepetition(history);
		};
		
		calcGameState(
			legals.size(),					// Number of legal moves
			history.current(),				// current position
			std::move(drawByRepetition));	// calculates draw by repetition using a GameHistory
	}

	void GameState::calcGameState(
		int nLegalMoves, 
		const Position & currPos,
		std::function<bool()>&& drawByRepetition)
	{
		// Blank out all values
		reset();

		const Position & position = currPos;
		const Board & board = position.board();

		// Who's turn is it anyway?
		bool isWhitesTurn = position.moveCounter().isWhitesTurn();

		// --- Player can't move (WINS or DRAWS) ---
		if (nLegalMoves == 0) {
			// There are no valid moves for current player. It is either a WIN or DRAW
			if (isWhitesTurn) {
				// --- WHITE'S TURN ---
				if (AttackChecker::isAttacked(board, board.m_whiteKing)) {
					// White's King is attacked and can't move.
					// Black wins
					this->player = PLAYER::WHITE;
					this->state = STATE::WIN;
					this->reason = REASON::CHECKMATE;
					return;
				}
				else {
					// White's King is not attacked but white can't move.
					// It's a draw
					this->state = STATE::DRAW;
					this->reason = REASON::STALEMATE;
					return;
				}
			}
			else {
				// --- BLACK'S TURN ---
				if (AttackChecker::isAttacked(board, board.m_blackKing)) {
					// Black's King is attacked and can't move.
					// White wins
					this->player = PLAYER::BLACK;
					this->state = STATE::WIN;
					this->reason = REASON::CHECKMATE;
					return;
				}
				else {
					// Black's King is not attacked and black can't move.
					// It's a draw
					this->state = STATE::DRAW;
					this->reason = REASON::STALEMATE;
					return;
				}
			}
		}

		// --- DRAWS ---

		// --- 50 Move Rule ---
		if (position.fiftyMoveRule().isDraw()) {
			this->state = STATE::DRAW;
			this->reason = REASON::FIFTY_MOVE_RULE;
			return;
		}

		// --- Draw by Repetition (Position has been reached 3 times) ---
		// Simply call this function that will figure out draw by repetition
		if (drawByRepetition()) {
			this->state = STATE::DRAW;
			this->reason = REASON::REPETITION;
			return;
		}

		// --- Insufficient Material ---
		if (isInsufficientMaterial(board)) {
			this->state = STATE::DRAW;
			this->reason = REASON::INSUFFICIENT_MATERIAL_ONLY;
			return;
		}
	}

	bool GameState::isDrawByRepetition(const Node * node)
	{
		const Position & currPos = node->position();

		const Node * nPtr = node;
		uint8_t matches = 0;
		
		while (nPtr != nullptr) {
			if (nPtr->position() == currPos) {
				matches++;

				if (matches >= 3) {
					return true;	// 3 matches found
				}
			}
		}

		return false;	// did not find 3 matches
	}

	bool GameState::isDrawByRepetition(const GameHistory & history)
	{
		const Position & currPos = history.current();

		GameHistory::const_iterator it = history.begin();
		GameHistory::const_iterator end = history.end();
		
		if (history.size()) {
			--end;
		}

		// Try to find 3 positions that match currPos
		auto nMatches = count(it, end, currPos);

		// 3 (or more) matches means draw by repetition.
		// We should only ever find 3 or less.
		return nMatches >= 3;
	}

	bool GameState::isInsufficientMaterial(const Board & board)
	{
		// Sufficient material:
		//  - atleast 1 pawn (either side)
		//	- atleast 1 queen (either side)
		//	- atleast 1 rook (either side)

		// Insufficient material:
		//	- King only vs King only
		//	- King vs King and 1 minor piece
		//	- King vs King and 2 Knights (only a draw in USCF)
		//	- King and 1 minor piece vs King and 1 minor Piece as long as:
		//		- White Bishop vs White Bishop
		//		- Black Bishop vs Black Bishop
		//		- Knight vs Knigh
		//		! Not WB vs BB nor BB vs WB !
		//		(only a draw in UCFS, See *)
		//	- King and 
		// * King and Bishop vs King and Bishop can result in checkmate as long as
		//	Bishops are on different colors. But checkmate cannot be forced without the
		//  help from the opponent.
		/*
					B L A C K (Minor Pieces)
				+---+---+---+---+
			W	|	| 0 | 1 | 2 |	I - insufficient material
			H	+---+---+---+---+	S - sufficient material
			I	| 0 | I | I | I |
			T	+---+---+---+---+
			E	| 1 | I | I | S |
				+---+---+---+---+
				| 2 | I | S | S |
				+---+---+---+---+



		*/
		// TODO: Might need to look at this logic more. 
		// the rules are very complicated and varry slightly.

		const bool INSUFFICIENT = true;
		const bool SUFFICIENT = false;

		int8_t nWhiteBishops = 0;
		int8_t nBlackBishops = 0;
		int8_t nWhiteKnights = 0;
		int8_t nBlackKnights = 0;
		bool whiteHasLightSquaredBishop = false;
		bool whiteHasDarkSquaredBishop = false;
		bool blackHasLightSquaredBishop = false;
		bool blackHasDarkSquaredBishop = false;

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 8; col++) {
				BoardSquare square{ row, col };
				Piece p = board.at(square);

				// Is there a piece here
				if (p.isOccupied()) {
					if (p.isPawn())		return SUFFICIENT;
					if (p.isRook())		return SUFFICIENT;
					if (p.isQueen())	return SUFFICIENT;
					if (p.isBishop()) {
						// --- BISHOP ---
						if (p.isWhite()) {
							// --- WHITE BISHOP ---
							nWhiteBishops++;

							if (square.isLightSquare()) {
								// --- WHITE LIGHT SQUARED BISHOP ---
								whiteHasLightSquaredBishop = true;
							}
							else {
								// --- WHITE DARK SQUARED BISHOP ---
								whiteHasDarkSquaredBishop = true;
							}
						}
						else {
							// --- BLACK BISHOP ---
							nBlackBishops++;

							if (square.isLightSquare()) {
								// --- BLACK LIGHT SQUARED BISHOP ---
								blackHasLightSquaredBishop = true;
							}
							else {
								// --- BLACK DARK SQUARED BISHOP ---
								blackHasDarkSquaredBishop = true;
							}
						}
					}
					if (p.isKnight()) {
						// --- KNIGHT ---
						if (p.isWhite()) {
							// --- WHITE KNIGHT ---
							nWhiteKnights++;
						}
						else {
							// --- BLACK KNIGHT ---
							nBlackKnights++;
						}
					}
				} // end if (p.isOccupied())
			} // end for (int col
		} // end for (int row

		// *** At this point we know that there are no: ***
		//	- Pawns
		//	- Queens
		//	- Rooks
		//	therefore a draw by insufficient material is possible
		// * Remember that there still might be 2 or more minor pieces on either side

		int8_t nWhiteMinors = nWhiteBishops + nWhiteKnights;
		int8_t nBlackMinors = nBlackBishops + nBlackKnights;

		// If both sides have any one of the following, 
		// and there are no pawns on the board:
		//	- Lone King vs Lone King:
		//		- 0 vs 0	(draw)
		//	- Long King vs King + 1 Minor:
		//		- 0 vs B	(draw)
		//		- B vs 0	(draw)
		//		- 0 vs N	(draw)
		//		- N vs 0	(draw)
		//	- King + 1 Minor vs King + 1 Minor:
		//		- B vs N	(draw)
		//		- B vs B	(draw iff Bs are on same color)
		//		- N vs N	(draw)
		//		- N vs B	(draw)
		if (nWhiteMinors <= 1 && nBlackMinors <= 1) {
			bool bishopsAreOnSameColor =
				(whiteHasLightSquaredBishop && blackHasLightSquaredBishop) ||
				(whiteHasDarkSquaredBishop && blackHasDarkSquaredBishop);

			if (bishopsAreOnSameColor) {
				return SUFFICIENT; // If bishops are on same color then checkmate is still possible
			}
			else {
				return INSUFFICIENT;
			}
		}

		// *** At this point we know that where are atleast 2+ minor pieces ***
		// Either:
		//	- White has 2+ minors
		//	- Black has 2+ minors
		//	- White and Black both have 2+ minors

		// 2 Knights vs Lone King is a draw
		if (nWhiteMinors == 0 && nBlackMinors == 2) {
			if (nBlackKnights == 2) {
				return INSUFFICIENT;	// draw
			}
		}
		else if (nWhiteMinors == 2 && nBlackMinors == 0) {
			if (nWhiteKnights == 2) {
				return INSUFFICIENT;	// draw
			}
		}
		else {
			// A forced checkmate is still possible
			return SUFFICIENT;
		}
	}

} // namespace forge