#pragma once

#include <algorithm>
#include <thread>

#include "forge/controllers/MCTS_Base.h"

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>

namespace forge
{
	class MCTS_Concurrent : public MCTS_Base<boost::atomic>
	{
	public:

		MCTS_Concurrent(size_t threadCount = 1) : m_nThreads(std::max(threadCount, (size_t) std::thread::hardware_concurrency())) {}
		MCTS_Concurrent(const MCTS_Concurrent &) = default;
		MCTS_Concurrent(MCTS_Concurrent &&) noexcept = default;
		~MCTS_Concurrent() noexcept = default;
		MCTS_Concurrent & operator=(const MCTS_Concurrent &) = default;
		MCTS_Concurrent & operator=(MCTS_Concurrent &&) noexcept = default;

		virtual std::string getNameVariant() const override { return "Concurrent"; }

		void threadCount(size_t threadCount) { m_nThreads = threadCount; }
		const size_t& threadCount() const { return m_nThreads; }

	protected:

		void solveOneThread();

	public:
		virtual void solve() override;

	protected:
		size_t m_nThreads = 0;	// 0 means search will determine thread count based on hardware concurrency
	};
} // namespace forge