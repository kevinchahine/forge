#include "PerformanceTester.h"

#include "MoveGenerator.h"

#include "MCTS_Solver_MT.h"

using namespace std;

namespace forge
{

	PerformanceTester::PerformanceTester()
	{
		reset();
	}

	void PerformanceTester::reset()
	{
		m_history.clear();
		m_history.emplace_back();
		m_history.back().reset();

		if (m_whitesSolver != nullptr) m_whitesSolver->reset();
		if (m_blacksSolver != nullptr) m_blacksSolver->reset();
	}

	GameState forge::PerformanceTester::runGame()
	{
		// --- Make sure we have controllers/solvers ---
		if (m_whitesSolver == nullptr) {
			cout << "PerformanceTester needs a controller for white" << endl;
			return GameState();
		}
		else {
			cout << "White controller: " << m_whitesSolver->getName() << '\n';
		}

		if (m_blacksSolver == nullptr) {
			cout << "PerformanceTester needs a controller for white" << endl;
			return GameState();
		}
		else {
			cout << "Black controller: " << m_blacksSolver->getName() << '\n';
		}

		// --- Prepare board for a new game ---
		reset();

		logger.start(
			m_whitesSolver->getName(),
			m_whitesSolver->getNameVariant(),
			m_whitesSolver->heuristicPtr()->name(),
			"v0.0");

		// --- Show board before playing ---
		if (m_viewPtr != nullptr) {
			m_viewPtr->show(this->position());
		}

		// --- Start Game ---

		GameState gstate;

		while (true)
		{
			// --- CONTROLLER ---
			// Who's turn is it anyways?
			SolverBase* currPlayer = nullptr;

			if (this->position().moveCounter().isWhitesTurn()) {
				// Its White's turn
				cout << "Whites turn...\n";
				currPlayer = m_whitesSolver.get();
			}
			else {
				// Its Black's turn
				cout << "Blacks turn...\n";
				currPlayer = m_blacksSolver.get();
			}

			// --- Prompt current player for a move ---
			MovePositionPair pair = currPlayer->getMove(this->position());
			cout << termcolor::green << "Player choose: " << pair.move << '\n';

			SearchMonitor& sm = currPlayer->searchMonitor();

			sm.print();

			logger.log(
				sm.nodeCount,
				sm.nodesPerSecond(),
				sm.plyCount,
				sm.plysPerSecond(),
				dynamic_cast<const MCTS_Solver_MT*>(currPlayer)->positionHashes().size(),
				sm.searchTime.elapsed());

			// Apply the move. 
			// TODO: Make sure move is legal before adding it.
			m_history.emplace_back(pair.position); // Its just that simple.

			// --- Check the game state ---
			gstate(m_history);
			if (gstate.state != GameState::STATE::CONTINUE) {
				cout << "Game over: " << gstate << '\n';
				break;
			}

			// --- VIEW ---
			if (m_viewPtr != nullptr) {
				m_viewPtr->show(m_history.current(), pair.move);
				///cout << "Press any key...";
				///cin.get();	// remove this
			}
		} // end while(true)

		return gstate;
	} // end runGame()
} // namespace forge