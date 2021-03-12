#pragma once

#include "Board.h"
#include "Clock.h"
#include "ControllerBase.h"
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
		// After calling reset() its good to set time controls for Clock. See Clock.h
		void reset();

		// Loop that runs game between two players. 
		// Returns when game finishes.
		// Blocking call.
		GameState runGame();

		Position & position() { return m_position; }
		const Position & position() const { return m_position; }

		template<typename CONTROLLER_T>
		void makeWhiteController() {
			m_whitesController = std::make_unique<CONTROLLER_T>();
		}

		template<typename CONTROLLER_T>
		void makeBlackController() {
			m_blacksController = std::make_unique<CONTROLLER_T>();
		}

		template<typename VIEW_T>
		void makeView() {
			m_viewPtr = std::make_unique<VIEW_T>();
		}

		Clock & clock() { return m_clock; }
		const Clock & clock() const { return m_clock; }

	private:
		GameState calcGameState() const;

	private:
		Position m_position;

		std::unique_ptr<ControllerBase> m_whitesController;
		std::unique_ptr<ControllerBase> m_blacksController;

		std::unique_ptr<ViewBase> m_viewPtr;

		Clock m_clock;
	};
} // namespace forge