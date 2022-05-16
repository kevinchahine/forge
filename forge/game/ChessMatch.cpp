#include "forge/game/ChessMatch.h"

#include "forge/movegen/MoveGenerator2.h"

using namespace std;

namespace forge
{
	ChessMatch::ChessMatch()
	{
		reset();
	}

	void ChessMatch::reset()
	{
		m_history.clear();
		m_history.emplace_back();
		m_history.back().position.reset();

		if (m_whitesController != nullptr) m_whitesController->reset();
		if (m_blacksController != nullptr) m_blacksController->reset();

		m_clock.resetAll();
	}

	GameState forge::ChessMatch::runGame()
	{
		cout << "Chess match is about to start...\n";

		// --- Make sure we have controllers/solvers ---
		if (m_whitesController == nullptr) {
			cout << "ChessMatch needs a controller for white" << endl;
			return GameState();
		}
		else {
			cout << "White controller: " << m_whitesController->getName() << '\n';
		}

		if (m_blacksController == nullptr) {
			cout << "ChessMatch needs a controller for black" << endl;
			return GameState();
		}
		else {
			cout << "Black controller: " << m_blacksController->getName() << '\n';
		}

		// --- Prepare board for a new game ---
		reset();

		// --- Show board before playing ---
		if (m_viewPtr != nullptr) {
			m_viewPtr->show(this->position());
		}

		// clock should have already been reset, syncronized and should now be paused.
		// Start the clock by "clicking" it.
		m_clock.click();

		GameState gstate;

		while (true)
		{
			// --- CONTROLLER ---
			// Who's turn is it anyways?
			ControllerBase * currPlayer = nullptr;

			if (this->position().moveCounter().isWhitesTurn()) {
				// White's turn
				cout << "Whites turn...\n";
				currPlayer = m_whitesController.get();
			}
			else {
				// Black's turn
				cout << "Blacks turn...\n";
				currPlayer = m_blacksController.get();
			}
			
			// --- Prompt current player for a move ---
			MovePositionPair pair = currPlayer->getMove(this->position());

			// --- VIEW ---
			// Show Board with highlights of legal moves but only if move was a partial move.
			if (pair.move.isPartial()) {
				if (m_viewPtr != nullptr) {
					MoveGenerator2 movegen;

					MoveList legals = movegen.generate(this->position());

					m_viewPtr->show(this->position(), legals);
				}

				// Prompt AGAIN for COMPLETE move
				pair = currPlayer->getMove(this->position());
			}
			
			// Apply the move. 
			// TODO: Make sure move is legal before adding it.
			m_history.emplace_back(pair); // Its just that simple.
			
			// Check the game state
			gstate(m_history);
			if (gstate.state != GameState::STATE::CONTINUE) {
				cout << "Game over: " << gstate << '\n';
				m_viewPtr->show(m_history.current().position);
				break;
			}

			// If displaying board, pause clock to prevent loosing time from next player.
			// If player is a computer, a few milliseconds lost can be a big deal. 
			if (true) {
				m_clock.stop();

				// Show Board
				if (m_viewPtr != nullptr) {
					m_viewPtr->show(m_history.current().position, pair.move);
				}

				m_clock.resume();
			}

			m_clock.click(); // Next players turn
		} // end while(true)

		return gstate;
	}

} // namespace forge