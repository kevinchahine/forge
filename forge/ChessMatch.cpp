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

		// clock should have already been reset, syncronized and should now be paused.
		// Start the clock by clicking it.
		m_clock.click();

		while (true)
		{
			Move m;

			// Who's turn is it?
			if (m_position.moveCounter().isWhitesTurn()) {
				// White's turn
				cout << "Whites turn...";
				m = m_whitesController->getMove(m_position);
			}
			else {
				// Black's turn
				cout << "Blacks turn...";
				m = m_blacksController->getMove(m_position);
			}

			// Show Board
			if (m_viewPtr != nullptr) {
				cout << '\n';
				m_viewPtr->show(m_position, MoveGenerator::generateLegalMovesFor(m_position, m.from()));
				cin.get();
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

				cout << "Press any key";
				cin.get();
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