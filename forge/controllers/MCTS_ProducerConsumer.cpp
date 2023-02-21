#include "MCTS_ProducerConsumer.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class

using namespace std;

mutex l_cout;

namespace forge
{
	// !!! children of MCTS_Nodes must be sorted in order of decending UCB scores for this to work !!!
	MCTS_Node::iterator selectMaster(MCTS_Node::iterator begin) {
		// Use queue to iterate a Breadth First Search (BFS) traversal.
		// As long as children nodes are sorted by UCB score in decending order, 
		// then the search will iterate in a Best First Search order traversal also.
		queue<MCTS_Node *> frontier;
		frontier.push(&(*begin));

		// Search tree for the "Best" leaf (Make sure we select a leaf)
		// Ignore nodes (and their subtrees) which are flagged.
		// If no leaves are found, return a null iterator.
		while (frontier.size()) {
			// Get next node
			MCTS_Node * node = frontier.front();
			frontier.pop();

			// Scan each child from greatest UCB score to least.
			// Skip the ones which are flagged.
			// In other words, look for the 1st unflagged leaf.
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
		/////l_cout.lock();
		/////cout << "Spawned thread " << boost::this_thread::get_id() << endl;
		/////l_cout.unlock();

		while (boost::this_thread::interruption_requested() == false) {

			MCTS_Node::iterator it;

			// --- 1.) State: Waiting for Work ---
			while (m_workA.pop(it) == false) {
				boost::this_thread::sleep_for(boost::chrono::microseconds(1));// TODO: optimize this to make it smarter
			}

			m_workASize--;

			/////l_cout.lock();
			/////cout << "Worker " << boost::this_thread::get_id() << ": Consuming work\t0x" << &(*it) << endl;
			/////cout << "queue size: " << m_workASize << endl;
			/////l_cout.unlock();

			// --- 2.) State: Working ---
			it.expand();// This code is not redundant

			// --- Evaluate ---
			//////EvalVisits ev = evaluate(it);

			// TODO: Do we need to backpropagate? I don't think so.

			// Do a MCTS for 10'000 nodes
			int nodeCount = 0;
			while (nodeCount++ < 1'000 && boost::this_thread::interruption_requested() == false) {
				MCTS_Node::iterator curr;

				// --- Selection ---
				curr = select(it);

				// --- Expand ---
				expand(curr);

				// --- Evaluate ---
				EvalVisits ev = evaluate(curr);

				// --- BackPropagate ---
				backPropagate(curr, (*it).parent(), ev);

				// --- Exit Condition ---
				// TODO: implement some exit condition here.
			} // while (true) 

			//cout << (*it).totalScore() << " " << (*it).nVisits() << endl;

			//////////l_cout.lock();
			//////////cout << "Worker " << boost::this_thread::get_id() << ": Consumed: \t0x" << &(*it) << endl;
			//////////l_cout.unlock();

			// --- 3.) State: Produce Work ---
			m_workB.push(it);
		} // while (true)

		/////l_cout.lock();
		/////cout << "Closing thread " << boost::this_thread::get_id() << endl;
		/////l_cout.unlock();
	} // searchOneThread(

	void MCTS_ProducerConsumer::solve() {
		solveTest();// TODO: remove this
		return;// TODO: remove this

		auto & sm = m_searchMonitor;

		// --- Clear the work queues ---
		while (m_workA.empty() == false) {
			MCTS_Node::iterator it;
			m_workA.pop(it);
		}

		while (m_workB.empty() == false) {
			MCTS_Node::iterator it;
			m_workB.pop(it);
		}

		m_workASize = 0;

		// Start with a small tree (sapling)
		m_nodeTree.expand();

		// --- Spawn Worker Threads ---
		size_t nThreads = (m_nThreads == 0 ? boost::thread::hardware_concurrency() : m_nThreads);
		vector<boost::thread> pool;
		pool.reserve(nThreads);

		for (size_t t = 0; t < nThreads; t++) {
			pool.emplace_back([&] () { this->searchOneThread(); });
		}

		while (!sm.exitConditionReached()) {
			// --- 1.) Selection (Produce) ---
			if (m_workASize < pool.size()/* + 2*/) {
				MCTS_Node::iterator leafIt = selectMaster(m_nodeTree.root());

				if (leafIt.isNotNull()) {
					MCTS_Node & leaf = (*leafIt);

					leaf.setFlag();

					/////////l_cout.lock();
					/////////cout << "Master: Produced work:\t0x" << &(*leafIt) << endl;
					/////////l_cout.unlock(); 

					m_workA.push(leafIt);

					m_workASize++;
				}
			}

			// --- 2.) Back Propagation (Consume) ---
			MCTS_Node::iterator it;

			bool succeeded = m_workB.pop(it);

			if (succeeded) {
				/////l_cout.lock();
				/////cout << "Master " << ": Pulled work\t0x" << &(*it) << endl;
				/////l_cout.unlock();

				MCTS_Node & node = (*it);

				//l_cout.lock();
				//cout << "Master: BP: " << node.totalScore() << ", " << node.nVisits() << endl;
				//l_cout.unlock();

				node.clearFlag();

				// Call back propagate
				EvalVisits ev{ node.totalScore(), node.nVisits() };
				backPropagate(it, m_nodeTree.root(), ev);

				sm.nodeCount += ev.visits;
			}
		} // while(

		// --- Join Threads ---
		for (boost::thread & t : pool) {
			t.interrupt();// Tell thread to return
		}

		for (boost::thread & t : pool) {
			t.join();// Wait for 
		}
	}

	void MCTS_ProducerConsumer::solveTest() {

		// --- Master Selection ---

		// Start with a small tree (sapling)
		m_nodeTree.expand();

		m_nodeTree.children().resize(4);// remove all but 4 nodes

		while (true) {
			MCTS_Node::iterator selection = selectMaster(m_nodeTree.root());

			// --- Worker Search ---

			MCTS_Node::iterator it = selection;
			while ((*selection).nVisits() < 1'000 && boost::this_thread::interruption_requested() == false) {
				MCTS_Node::iterator curr;

				// --- Selection ---
				curr = select(it);

				// --- Expand ---
				expand(curr);

				// --- Evaluate ---
				EvalVisits ev = evaluate(curr);

				// --- BackPropagate ---
				backPropagate(curr, (*it).parent(), ev);

				// --- Exit Condition ---
				// TODO: implement some exit condition here.
			} // while (true) 

			// --- Master BackPropagate ---
			MCTS_Node & node = (*it);

			node.clearFlag();

			// Call back propagate
			// TODO: Fix Bug: PB is not suppost to increment nVisits of the selected node
			EvalVisits ev{ -node.totalScore(), node.nVisits() };
			it.toParent();
			backPropagate(it, m_nodeTree.root(), ev);

			continue;
		} // end while()
	} // class MCTS_ProducerConsumer
} // namespace forge

