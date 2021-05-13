#pragma once

#include "SolverBase.h"
#include "GameHistory.h"
#include "GameState.h"
#include "Position.h"
#include "ViewBase.h"
#include "PerformanceLogger.h"

#include <memory>

namespace forge
{
	class PerformanceTester
	{
	public:
		PerformanceTester();
		PerformanceTester(const PerformanceTester &) = default;
		PerformanceTester(PerformanceTester &&) noexcept = default;
		virtual ~PerformanceTester() noexcept = default;
		PerformanceTester & operator=(const PerformanceTester &) = default;
		PerformanceTester & operator=(PerformanceTester &&) noexcept = default;

		// After calling reset() its good to set time controls for Clock. See Clock.h
		void reset();

		// Loop that runs game between two players. 
		// Returns when game finishes.
		// Blocking call.
		GameState runGame();

		Position & position() { return m_history.current(); }
		const Position & position() const { return m_history.current(); }

		std::unique_ptr<SolverBase> & whiteSolver() { return m_whitesSolver; }
		const std::unique_ptr<SolverBase> & whiteSolver() const { return m_whitesSolver; }

		std::unique_ptr<SolverBase> & blackSolver() { return m_blacksSolver; }
		const std::unique_ptr<SolverBase> & blackSolver() const { return m_blacksSolver; }

		std::unique_ptr<ViewBase> & view() { return m_viewPtr; }
		const std::unique_ptr<ViewBase> & view() const { return m_viewPtr; }

	protected:
		GameHistory m_history;
		PerformanceLogger logger;

		std::unique_ptr<SolverBase> m_whitesSolver;
		std::unique_ptr<SolverBase> m_blacksSolver;

		std::unique_ptr<ViewBase> m_viewPtr;
	};
} // namespace forge