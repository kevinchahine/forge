#pragma once

#include "Board.h"
#include "Clock.h"
#include "ControllerBase.h"
#include "GameHistory.h"
#include "GameState.h"
#include "Position.h"
#include "ViewBase.h"

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

		Position & position() { return m_history.current(); }
		const Position & position() const { return m_history.current(); }

		template<typename CONTROLLER_T>
		void makeWhiteController() {
			m_whitesController = std::make_unique<CONTROLLER_T>();
		}

		void setWhiteController(std::unique_ptr<ControllerBase> && whiteController) { m_whitesController = std::move(whiteController); }

		std::unique_ptr<ControllerBase> & whiteController() { return m_whitesController; }
		const std::unique_ptr<ControllerBase> & whiteController() const { return m_whitesController; }

		template<typename CONTROLLER_T>
		void makeBlackController() {
			m_blacksController = std::make_unique<CONTROLLER_T>();
		}
		
		void setBlackController(std::unique_ptr<ControllerBase> && blackController) { m_blacksController = std::move(blackController); }

		std::unique_ptr<ControllerBase> & blackController() { return m_blacksController; }
		const std::unique_ptr<ControllerBase> & blackController() const { return m_blacksController; }

		template<typename VIEW_T>
		void makeView() {
			m_viewPtr = std::make_unique<VIEW_T>();
		}

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