#pragma once

#include "forge/controllers/SolverBase.h"
#include "forge/search/MCTS_Node.h"

#include <set>

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lockfree/lockfree_forward.hpp>

namespace forge
{
	class MCTS_ProducerConsumer : public SolverBaseTemplate<boost::atomic>
	{
		using WorkQueue = boost::lockfree::queue<forge::MCTS_Node::iterator>;

	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "ProducerConsumer"; }

		///const MCTS_Node& nodeTree() const { return m_nodeTree; }

		size_t& threadCount() { return m_nThreads; }
		const size_t& threadCount() const { return m_nThreads; }

	protected:

		// TODO: Could make this constant
		MovePositionPair selectBestMove();

		void select(WorkQueue & selectionQueue);

		void expandAndEvaluate(WorkQueue& selectionQueue, WorkQueue& backPropQueue);

		void backpropagate(WorkQueue& backPropQueue);

	public:
		MovePositionPair solve(const Position& position);

	protected:
		MCTS_Node m_nodeTree;

		size_t m_nThreads = 0;	// 0 means search will determine thread count based on hardware concurrency
	};
} // namespace forge