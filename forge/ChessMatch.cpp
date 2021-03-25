#include "ChessMatch.h"

#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	void ChessMatch::reset()
	{
		m_position.reset();

		m_whitesController.reset();
		m_blacksController.reset();

		m_clock.resetAll();
	}

	GameState forge::ChessMatch::runGame()
	{
		// Make sure we have controllers/solvers.
		if (m_whitesController == nullptr) {
			cout << "ChessMatch needs a controller for white" << endl;
			return GameState();
		}
		else {
			cout << "White controller: " << m_whitesController->getName() << '\n';
		}

		if (m_blacksController == nullptr) {
			cout << "ChessMatch needs a controller for white" << endl;
			return GameState();
		}
		else {
			cout << "Black controller: " << m_blacksController->getName() << '\n';
		}

		// --- Set up board ---
		m_position.reset();

		// --- Show board before playing ---
		if (m_viewPtr != nullptr) {
			m_viewPtr->show(m_position);
		}

		// clock should have already been reset, syncronized and should now be paused.
		// Start the clock by clicking it.
		m_clock.click();

		while (true)
		{
			Move m;

			// --- CONTROLLER ---
			// Who's turn is it?
			ControllerBase * currPlayer = nullptr;

			if (m_position.moveCounter().isWhitesTurn()) {
				// White's turn
				cout << "Whites turn...";
				currPlayer = m_whitesController.get();
			}
			else {
				// Black's turn
				cout << "Blacks turn...";
				currPlayer = m_blacksController.get();
			}

			m = currPlayer->getMove(m_position);

			// --- VIEW ---
			// Show Board with highlights of legal moves but only if move was a partial move.
			if (m.isPartial()) {
				if (m_viewPtr != nullptr) {
					m_viewPtr->show(m_position, MoveGenerator::generateLegalMovesFor(m_position, m.from()));
				}

				// Prompt again for complete move
				Move toMove = currPlayer->getMove(m_position);

				m = Move{ m.from(), toMove.from(), toMove.promotion() };
				// Now we have the complete move
			}

			cout << m << "\n";
			m_position.applyMoveSafe(m);
			
			// If displaying board, pause clock to prevent loosing time from next player.
			// If player is a computer, a few milliseconds lost can be a big deal. 
			if (true) {
				m_clock.stop();

				// Show Board
				if (m_viewPtr != nullptr) {
					m_viewPtr->show(m_position, m);
				}

				m_clock.resume();
			}

			m_clock.click();
		}

		return GameState();
	}

	GameState ChessMatch::calcGameState() const
	{
		GameState gState;

		gState.state = GameState::STATE::CONTINUE;

		// Whos turn is it?
		if (m_position.moveCounter().isWhitesTurn()) {
			// white's turn.

			// Can white's king move?
			// TODO:
		}
		else {
			// blacks turn.

			// Can black's king move?
			// TODO:

		}

		return gState;
	}
} // namespace forge