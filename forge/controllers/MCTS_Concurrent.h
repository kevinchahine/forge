#pragma once

#include "forge/controllers/MCTS_Base.h"

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>

namespace forge
{
	class MCTS_Concurrent : public MCTS_Base<boost::atomic>
	{
	public:
		virtual std::string getNameVariant() const override { return "Concurrent"; }

		size_t& threadCount() { return m_nThreads; }
		const size_t& threadCount() const { return m_nThreads; }

	protected:

		void solveOneThread();

	public:
		virtual MovePositionPair solve() override;

	protected:
		size_t m_nThreads = 0;	// 0 means search will determine thread count based on hardware concurrency
	};
} // namespace forge