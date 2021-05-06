#pragma once

#include "ControllerBase.h"

#include "HeuristicBase.h"

#include <vector>
#include <type_traits>	// for is_base_of()
#include <typeinfo>

namespace forge
{
	class SolverBase : public ControllerBase
	{
	public:

		const std::unique_ptr<HeuristicBase> & heuristicPtr() const { return m_heuristicPtr; }
		std::unique_ptr<HeuristicBase> & heuristicPtr() { return m_heuristicPtr; }

		template<typename T>
		void makeHeuristic();

		template<typename T>
		void makeHeuristic(const std::vector<heuristic_t> & weights);

	protected:
		std::unique_ptr<HeuristicBase> m_heuristicPtr;
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

		m_heuristicPtr->weights() = weights;
	}
} // namespace forge