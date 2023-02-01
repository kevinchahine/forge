#pragma once

#include "forge/controllers/MCTS_Base.h"
#include "forge/search/MCTS_Node.h"

#include <set>

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lockfree/lockfree_forward.hpp>

namespace forge
{
	class MCTS_ProducerConsumer : public MCTS_Base<boost::atomic>
	{
		using WorkQueueA = boost::lockfree::queue<MCTS_Node::iterator>;
		using WorkQueueB = boost::lockfree::queue<NodeItEvalVisits>;

	public:
		virtual std::string getNameVariant() const override { return "ProducerConsumer"; }

		size_t& threadCount() { return m_nThreads; }
		const size_t& threadCount() const { return m_nThreads; }

	public:
		MovePositionPair solve(const Position& position);

	protected:
		size_t m_nThreads = 0;	// 0 means search will determine thread count based on hardware concurrency
	};
} // namespace forge