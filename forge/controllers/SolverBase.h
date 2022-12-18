#pragma once

#include "forge/controllers/ControllerBase.h"
#include "forge/heuristic/Base.h"
#include "forge/search/SearchMonitor.h"

#include <forge/core/MoveGenerator2.h>

#include <vector>
#include <type_traits>	// for is_base_of()
#include <typeinfo>

namespace forge
{
	class SolverBase : public ControllerBase
	{
	public:
		virtual void printSearchMonitor() const = 0;
	};

	template< template< typename T, typename... > class AC, typename... Args >
	class SolverBaseTemplate : public SolverBase
	{
	public:
		SolverBaseTemplate() = default;
		SolverBaseTemplate(const SolverBaseTemplate& solver) :
			ControllerBase(solver),
			m_heuristicPtr(solver.m_heuristicPtr->clone()),
			m_searchMonitor(solver.m_searchMonitor)
		{	}
		SolverBaseTemplate(SolverBaseTemplate&&) noexcept = default;
		virtual ~SolverBaseTemplate() noexcept = default;
		SolverBaseTemplate& operator=(const SolverBaseTemplate& solver) 
		{
			static_cast<ControllerBase&>(*this) = solver;
			m_heuristicPtr = solver.m_heuristicPtr->clone();
			m_searchMonitor = solver.m_searchMonitor;
			
			return *this;
		}
		SolverBaseTemplate& operator=(SolverBaseTemplate&&) noexcept = default;

		const std::unique_ptr<heuristic::Base> & heuristicPtr() const { return m_heuristicPtr; }
		std::unique_ptr<heuristic::Base> & heuristicPtr() { return m_heuristicPtr; }

		template<typename T>
		void makeHeuristic() 
		{
			static_assert(std::is_base_of<heuristic::Base, T>::value,
				"heuristic::Base is not a base class of type T");
		
			m_heuristicPtr = std::make_unique<T>();
		}
		
		template<typename T>
		void makeHeuristic(const std::vector<heuristic_t> & weights)
		{
			static_assert(std::is_base_of<heuristic::Base, T>::value,
				"heuristic::Base is not a base class of type T");

			m_heuristicPtr = std::make_unique<T>();

			//TODO: I don't know.
			//m_heuristicPtr->weights() = weights;
		}
		
		template<typename T>
		void makeHeuristic(const std::string& file_name)
		{
			static_assert(std::is_base_of<heuristic::Base, T>::value,
				"heuristic::Base is not a base class of type T");

			m_heuristicPtr = std::make_unique<T>(file_name);
		}

		virtual void printSearchMonitor() const override { m_searchMonitor.print(); }

		const SearchMonitorTemplate<AC>& searchMonitor() const { return m_searchMonitor; }
		SearchMonitorTemplate<AC>& searchMonitor() { return m_searchMonitor; }

	protected:
		std::unique_ptr<heuristic::Base> m_heuristicPtr;

	public:
		SearchMonitorTemplate<AC> m_searchMonitor;
	};
} // namespace forge