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
	template< template< typename T, typename... > class AC, typename... Args >
	class SolverBase : public ControllerBase
	{
	public:
		SolverBase() = default;
		SolverBase(const SolverBase& solver) :
			ControllerBase(solver),
			m_heuristicPtr(solver.m_heuristicPtr->clone()),
			m_searchMonitor(solver.m_searchMonitor)
		{	}
		SolverBase(SolverBase&&) noexcept = default;
		virtual ~SolverBase() noexcept = default;
		SolverBase& operator=(const SolverBase& solver) 
		{
			static_cast<ControllerBase&>(*this) = solver;
			m_heuristicPtr = solver.m_heuristicPtr->clone();
			m_searchMonitor = solver.m_searchMonitor;
			
			return *this;
		}
		SolverBase& operator=(SolverBase&&) noexcept = default;

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

		const SearchMonitor & searchMonitor() const { return m_searchMonitor; }
		SearchMonitor & searchMonitor() { return m_searchMonitor; }

	protected:
		std::unique_ptr<heuristic::Base> m_heuristicPtr;

	public:
		SearchMonitorTemplate<AC> m_searchMonitor;
	};
} // namespace forge