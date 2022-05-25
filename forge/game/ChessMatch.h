#pragma once

#include "forge/core/Board.h"
#include "forge/core/Position.h"
#include "forge/controllers/ControllerBase.h"
#include "forge/game/GameHistory.h"
#include "forge/game/GameState.h"
#include "forge/views/ViewBase.h"
#include "forge/time/Clock.h"

#include <memory>

namespace forge
{
	// Runs a game of chess. Intended to be used in a stand alone mode,
	// when playing two humans, two engines or one human and one engine.
	// Not intended to be used which UCI.
	class ChessMatch
	{
	public:
		ChessMatch();
		ChessMatch(const ChessMatch &) = default;
		ChessMatch(ChessMatch &&) noexcept = default;
		~ChessMatch() noexcept = default;
		ChessMatch & operator=(const ChessMatch &) = default;
		ChessMatch & operator=(ChessMatch &&) noexcept = default;

		// After calling reset() its good to set time controls for Clock. See Clock.h
		void reset();

		// Loop that runs game between two players. 
		// Returns when game finishes.
		// Blocking call.
		GameState runGame();

		GameHistory & history() { return m_history; };
		const GameHistory & history() const { return m_history; };

		Position & position() { return m_history.current().position; }
		const Position & position() const { return m_history.current().position; }

		// --- Controllers (and Solvers) ---
		template<typename CONTROLLER_T>
		void makeWhiteController() { m_whitesController = std::make_unique<CONTROLLER_T>();	}

		void setWhiteController(std::unique_ptr<ControllerBase> && whiteController) { m_whitesController = std::move(whiteController); }

		std::unique_ptr<ControllerBase> & whiteController() { return m_whitesController; }
		const std::unique_ptr<ControllerBase> & whiteController() const { return m_whitesController; }

		template<typename CONTROLLER_T>
		void makeBlackController() { m_blacksController = std::make_unique<CONTROLLER_T>();	}
		
		void setBlackController(std::unique_ptr<ControllerBase> && blackController) { m_blacksController = std::move(blackController); }

		std::unique_ptr<ControllerBase> & blackController() { return m_blacksController; }
		const std::unique_ptr<ControllerBase> & blackController() const { return m_blacksController; }

		// --- Views ---
		template<typename VIEW_T>
		void makeView() {
			m_viewPtr = std::make_unique<VIEW_T>();
		}

		// --- Time Control ---

		Clock & clock() { return m_clock; }
		const Clock & clock() const { return m_clock; }

	private:
		GameHistory m_history;

		std::unique_ptr<ControllerBase> m_whitesController;
		std::unique_ptr<ControllerBase> m_blacksController;

		std::unique_ptr<ViewBase> m_viewPtr;

		Clock m_clock;
	};
} // namespace forge