#include "MCTS_ProducerConsumer.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class

using namespace std;

namespace forge
{
	// !!! children of MCTS_Nodes must be sorted in order of decending UCB scores for this to work !!!
	MCTS_Node::iterator selectMaster(MCTS_Node::iterator begin) {
		// Use stack to iterate a DFS traversal.
		// As long as children nodes are sorted by UCB score in decending order, 
		// then the search will iterate in a Best First Search (BFS) order traversal.
		stack<MCTS_Node *> frontier;
		frontier.push(&(*begin));

		// Search tree for the "Best" leaf
		// Ignore nodes (and their subtrees) which are flagged.
		// If no leaves are found, return a null iterator.
		while (frontier.size()) {
			// Get next node
			MCTS_Node * node = frontier.top();
			frontier.pop();

			// Scan each child from greatest UCB score to least.
			// Skip the ones which are flagged.
			// Look for the 1st unflagged leaf.
			for (const auto & child : node->children()) {
				if (child->flagIsCleared()) {
					if (child->isLeaf()) {
						// We have found the best unflagged leaf!!! :)
						return MCTS_Node::iterator(child.get());
					}
					else {
						// This node is not a leaf. Keep looking.
						frontier.push(child.get());
					}
				}
			}
		}

		// No unflagged leaves were found :(
		return MCTS_Node::iterator{};// null iterator
	}

	void MCTS_ProducerConsumer::searchOneThread() {
		while (boost::this_thread::interruption_requested() == false) {
			MCTS_Node::iterator it;

			// --- 1.) State: Waiting for Work ---
			while (m_workA.pop(it) == false) {
				boost::this_thread::sleep_for(boost::chrono::microseconds(10));// TODO: optimize this to make it smarter
			}

			m_workASize--;

			// --- 2.) State: Working ---
			it.expand();// This code is not redundant

			// Do a MCTS for 10'000 nodes
			int nodeCount = 0;
			while (nodeCount++ < 10'000 && boost::this_thread::interruption_requested() == false) {
				MCTS_Node::iterator curr = it;

				// --- Selection ---
				curr = select(curr);

				// --- Expand ---
				expand(curr);

				// --- Evaluate ---
				EvalVisits ev = evaluate(curr);

				// --- BackPropagate ---
				backPropagate(curr, m_nodeTree.root(), ev);

				// --- Exit Condition ---
				// TODO: implement some exit condition here.
			} // while (true) 

			// --- 3.) State: Produce Work ---
			m_workB.push(it);
		} // while (true)
	} // searchOneThread(

	void MCTS_ProducerConsumer::solve() {
		auto & sm = m_searchMonitor;

		// --- Spawn Worker Threads ---
		vector<boost::thread> pool;
		pool.reserve(m_nThreads);

		for (size_t t = 0; t < m_nThreads; t++) {
			pool.emplace_back([&] () { this->searchOneThread(); });
		}

		while (!sm.exitConditionReached()) {
			// --- 1.) Select Leaf Nodes (Produce) ---
			if (m_workASize < m_nThreads) {
				MCTS_Node::iterator leafIt = selectMaster(m_nodeTree.root());

				if (leafIt.isNotNull()) {
					MCTS_Node & leaf = (*leafIt);

					leaf.setFlag();

					m_workA.push(leafIt);

					m_workASize++;
				}
			}

			// --- 2.) Back Propagate (Consume) ---
			MCTS_Node::iterator it;

			bool succeeded = m_workB.pop(it);

			if (succeeded) {
				MCTS_Node & node = (*it);

				node.clearFlag();

				// Call back propagate
				EvalVisits ev{ node.totalScore(), node.nVisits() };
				backPropagate(it, m_nodeTree.root(), ev);
			}
		} // while(

		// --- Join Threads ---
		for (boost::thread & t : pool) {
			t.interrupt();
			
			t.join();
		}
	}
} // namespace forge

