#pragma once

#include "forge/controllers/ControllerBase.h"
#include "forge/heuristics/HeuristicBase.h"
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
		SolverBase() = default;
		SolverBase(const SolverBase& solver);
		SolverBase(SolverBase&&) noexcept = default;
		virtual ~SolverBase() noexcept = default;
		SolverBase& operator=(const SolverBase& solver);
		SolverBase& operator=(SolverBase&&) noexcept = default;

		const std::unique_ptr<HeuristicBase> & heuristicPtr() const { return m_heuristicPtr; }
		std::unique_ptr<HeuristicBase> & heuristicPtr() { return m_heuristicPtr; }

		template<typename T>
		void makeHeuristic();

		template<typename T>
		void makeHeuristic(const std::vector<heuristic_t> & weights);

		template<typename T>
		void makeHeuristic(const std::string& file_name);

		const SearchMonitor & searchMonitor() const { return m_searchMonitor; }
		SearchMonitor & searchMonitor() { return m_searchMonitor; }

	protected:
		std::unique_ptr<HeuristicBase> m_heuristicPtr;

	public:
		SearchMonitor m_searchMonitor;
	};

	template<typename T>
	inline void SolverBase::makeHeuristic()
	{
		static_assert(std::is_base_of<HeuristicBase, T>::value,
			"HeuristicBase is not a base class of type T");

		m_heuristicPtr = std::make_unique<T>();
	}

	template<typename T>
	inline void SolverBase::makeHeuristic(const std::vector<heuristic_t>& weights)
	{
		static_assert(std::is_base_of<HeuristicBase, T>::value,
			"HeuristicBase is not a base class of type T");

		m_heuristicPtr = std::make_unique<T>();

		//TODO: I don't know.
		//m_heuristicPtr->weights() = weights;
	}
	
	template<typename T>
	inline void SolverBase::makeHeuristic(const std::string& file_name)
	{
		static_assert(std::is_base_of<HeuristicBase, T>::value,
			"HeuristicBase is not a base class of type T");
		
		m_heuristicPtr = std::make_unique<T>(file_name);
	}
} // namespace forge