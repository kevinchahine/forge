#include "MoveList.h"

#include "AttackChecker.h"

using namespace std;

namespace forge
{
	///#ifndef FORGE_DEBUG_PUSH_IF_LEGAL_MOVE
	///#define FORGE_DEBUG_PUSH_IF_LEGAL_MOVE(CURR_POSITION, MOVE, PIECE_CONDITIONAL, THIS_FUNCTION) \
	///		if (CURR_POSITION.board().empty()[MOVE.from()]) { \
	///			cout << "Error " << THIS_FUNCTION << ": from can't be empty\n"; } \ 
	///		else if (CURR_POSITION.board().occupied()[MOVE.to()]) { \
	///			cout << "Error " << THIS_FUNCTION << ": to can't be occupied\n"; \
	///		} \
	///		else if (CURR_POSITION.board().at(MOVE.from()).PIECE_CONDITIONAL()) { \
	///			cout << "Error " << THIS_FUNCTION << ": from must point to a PIECE_CONDITIONAL\n"; \
	///		} 
	///#endif // FORGE_DEBUG_PUSH_IF_LEGAL_MOVE

	void MoveList::pushIfLegalQBNMove(const Position & currPos, Move move)
	{
		cout << "YO\n";
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.moveQBNR(move);

		// 3.) --- Make sure it was a legal move ---
		const bool isWhiteMoving = currPos.moveCounter().isWhitesTurn();
		if (AttackChecker::isAttacked(this->back().position.board(), !isWhiteMoving)) {
			cout << "Great illegal move found\n";
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalQBNCapture(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.captureWithQBNR(move);

		// 3.) --- Make sure it was a legal move ---
		const bool isWhiteMoving = currPos.moveCounter().isWhitesTurn();
		if (AttackChecker::isAttacked(this->back().position.board(), !isWhiteMoving)) {
			cout << "Great illegal move found\n";
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalWhitePawnMove(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.moveWhitePawn(move);

		// 3.) --- Make sure it was a legal move ---
		if (AttackChecker::isAttacked(this->back().position.board(), true)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalBlackPawnMove(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.moveBlackPawn(move);

		// 3.) --- Make sure it was a legal move ---
		if (AttackChecker::isAttacked(this->back().position.board(), false)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalWhitePawnCapture(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.captureWithWhitePawn(move);

		// 3.) --- Make sure it was a legal move ---
		if (AttackChecker::isAttacked(this->back().position.board(), true)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalBlackPawnCapture(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.captureWithBlackPawn(move);

		// 3.) --- Make sure it was a legal move ---
		if (AttackChecker::isAttacked(this->back().position.board(), false)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalRookMove(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.moveRook(move);

		// 3.) --- Make sure it was a legal move ---
		const bool isWhiteMoving = currPos.board().isWhite(move.from());
		if (AttackChecker::isAttacked(this->back().position.board(), isWhiteMoving)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalRookCapture(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		this->back().position.captureWithRook(move);

		// 3.) --- Make sure it was a legal move ---
		const bool isWhiteMoving = currPos.board().isWhite(move.from());
		if (AttackChecker::isAttacked(this->back().position.board(), isWhiteMoving)) {
			this->pop_back(); // Illegal Move
		}
	}
	
	void MoveList::pushIfLegalKingMove(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		if (this->back().position.board().isWhite(move.from())) {
			this->back().position.moveWhiteKing(move.to());
			
			// 3.) --- Make sure it was a legal move ---
			if (AttackChecker::isAttacked(this->back().position.board(), true)) {
				// Illegal move
				this->pop_back();
			}
		}
		else {
			this->back().position.moveBlackKing(move.to());

			// 3.) --- Make sure it was a legal move ---
			if (AttackChecker::isAttacked(this->back().position.board(), false)) {
				// Illegal move
				this->pop_back();
			}
		}
	}
	
	void MoveList::pushIfLegalKingCapture(const Position & currPos, Move move)
	{
		// 1.) --- Copy move and position to back of container ---
		this->emplace_back(move, currPos);

		// 2.) --- Apply move to the pushed position ---
		if (this->back().position.board().isWhite(move.from())) {
			// --- White King is moving ---
			this->back().position.captureWithWhiteKing(move.to());
			
			// 3.) --- Make sure it was a legal move ---
			if (AttackChecker::isAttacked(this->back().position.board(), true)) {
				this->pop_back();	// Illegal move
			}
		}
		else {
			// --- Black King is moving ---
			this->back().position.captureWithBlackKing(move.to());
			
			// 3.) --- Make sure it was a legal move ---
			if (AttackChecker::isAttacked(this->back().position.board(), false)) {
				this->pop_back();	// Illegal move
			}
		}
	}
} // namespace forge