#include "KeyboardController.h"

#include "MoveGenerator2.h"
//#include "MoveGenerator.h"

#include <algorithm>
#include <functional>

namespace forge
{
	void KeyboardController::reset()
	{
		m_move = Move{ BoardSquare{ 'a', '1' }, BoardSquare{ 'a', '2' } };
	}

	MovePositionPair KeyboardController::getMove(const Position & position)
	{
		// TODO: Consider PGN moves here since we have the Position
		// TODO: Consider offering a list of valid moves if user asks maybe?

		// Will contain the move and resulting position that the user will select.
		MovePositionPair ret;

		// Current user input
		Move input;

		// List of legal moves for current player
		MoveGenerator2 movegen;
		MoveList legalMoves = movegen.generate(position);

		if (legalMoves.empty()) {
			cout << "Error: " << __FUNCTION__ << " this player has no legal moves to make.\n";
			return ret;
		}

		cout << "\nYour legal moves: ";
		legalMoves.print();

		// Where are 'our' pieces? (Pieces of player who is making the move).
		BitBoard ours = (
			position.moveCounter().isWhitesTurn() ?
			position.board().whites() :
			position.board().blacks());

		do {
			// --- Prompt user for a move ---
			cout << "Enter move (ex " << legalMoves.front().move << "): ";
			cin >> input;

			// Is input a partial move or full move?
			if (input.isPartial()) {
				cout << "You entered a partial move\n";
				// Input is a PARTIAL move. 

				// *** At this point we can assume that m_move is ***
				// *** either partial or full move ***

				// Was the last move a partial move also?
				if (m_move.isPartial()) {
					// Yes, last move was also a partial move
					// meaning it only had the 'from' component.
					// Combine these to form a full move.
					input.from(m_move.from());

					// *** Now we have a full move ***

					// Make sure its legal
					MoveList::const_iterator it = findMatchingMove(input, legalMoves);
					if (it == legalMoves.end()) {
						// --- Input is illegal ---
						cout << input << " is not a legal move. There are " 
							<< legalMoves.size() << " legal moves that you can make."
							<< "Try again: ";
						continue;
					}
					else {
						// --- Input is legal ---
						m_move = it->move;
						ret.move = it->move;
						ret.position = it->position;
						break;	// We've found our move. Great :)
					}
				}
				else {

					// Last move was not a partial move, that means input
					// is the 'from' components of what is to be a full move.

					// Make sure the partial move points to one of the players own pieces
					if (ours[input.from()] == false) {
						// Partial move does not point to one of players own pieces.
						// Prompt user to try again.
						cout << "You can only move your pieces. Try again: ";
						continue;
					}

					MoveList::const_iterator it = findMatchingMove(input, legalMoves);
					if (it == legalMoves.end()) {
						cout << "That piece can't move. "
							<< "There are " << legalMoves.size()
							<< " legal moves that you can make. Try again: ";
						continue;
					}

					// Partial move does point to one of players own pieces. Good :)
					// Return the partial move and current position.
					// Because a partial move doesn't change a position 
					// (only a full move does that), return the current 
					// unchanged position as it is.
					m_move = input;
					ret.move = input;
					ret.position = position;
					break;
				}
			}
			else {
				// *** Input is a FULL move ***
				MoveList::const_iterator it = findMatchingMove(input, legalMoves);
				if (it != legalMoves.end()) {
					// --- Legal Move ---
					m_move = it->move;
					ret.move = it->move;
					ret.position = it->position;
					break;
				}
				else {
					// --- Illegal Move ---
					cout << input << " is not a legal moves. Try again: ";
					continue;
				}
			}
		} while (true);

		return ret;
	}

	MoveList::const_iterator KeyboardController::findMatchingMove(Move move, const MoveList & legalMoves)
	{
		std::function<bool(const MovePositionPair &)> compPartialMoves =
			[&](const MovePositionPair & pair) -> bool { return pair.move.from() == move.from(); };

		std::function<bool(const MovePositionPair &)> compFullMoves =
			[&](const MovePositionPair & pair) -> bool { return pair.move == move; };

		std::function<bool(const MovePositionPair &)> compare = (
			move.isPartial() ?
			compPartialMoves :
			compFullMoves
			);

		///MoveList::const_iterator it;
		///for (it = legalMoves.begin(); it != legalMoves.end(); ++it) {
		///	if (compare(*it)) break;
		///}
		MoveList::const_iterator it = find_if(
			legalMoves.begin(),
			legalMoves.end(),
			compare);

		return it;
	}
} // namespace forge