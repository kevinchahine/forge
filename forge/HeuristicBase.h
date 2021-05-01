#pragma once

#include "Position.h"

namespace forge
{
	// Type the heuristics value will be stored in
	// Unit is in centi pawns (1/100 pawn)
	using heuristic_t = int;

	class HeuristicBase
	{
	public:
		virtual heuristic_t eval(const Position & pos) = 0;

		virtual std::unique_ptr<HeuristicBase> clone() const = 0;

		template<typename T>
		std::unique_ptr<T> cloneAs() const;

		std::vector<heuristic_t> & weights() { return m_weights; }
		const std::vector<heuristic_t> & weights() const { return m_weights; }

	protected:
		std::vector<heuristic_t> m_weights;
	};

	template<typename T>
	inline std::unique_ptr<T> HeuristicBase::cloneAs() const
	{
		///std::unique_ptr<HeuristicBase> ptrBase = this->clone();
		///
		///std::unique_ptr<T> ptrDerived = move(ptrBase);

		std::unique_ptr<T> ptr = this->clone();

		return ptr;// ptrDerived;
	}
} // namespace forge