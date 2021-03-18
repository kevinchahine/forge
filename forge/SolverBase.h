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

		template<typename T>
		void makeHeuristic();

		template<typename T>
		void makeHeuristic(const std::vector<double> & weights);

	protected:
		std::unique_ptr<HeuristicBase> heuristicPtr;
	};

	template<typename T>
	inline void SolverBase::makeHeuristic()
	{
		static_assert(std::is_base_of<HeuristicBase, T>::value,
			"HeuristicBase is not a base class of type T");

		heuristicPtr = std::make_unique<T>();
	}

	template<typename T>
	inline void SolverBase::makeHeuristic(const std::vector<double>& weights)
	{
		static_assert(std::is_base_of<HeuristicBase, T>::value,
			"HeuristicBase is not a base class of type T");

		heuristicPtr = std::make_unique<T>();

		heuristicPtr->weights() = weights;
	}
} // namespace forge