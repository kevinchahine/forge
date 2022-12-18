#include "MCTS_Concurrent.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class

using namespace std;

namespace forge
{
	void MCTS_Concurrent::solveOneThread() {
		MCTS_Node::iterator curr = m_nodeTree.root();

		//while (true) {
		//	if (curr.isLeaf()) {
		//		heuristic_t eval = 0;
		//
		//		(*curr).mutex().lock();// ~~~~~~~~~~~~~~~~ Lock ~~~~
		//
		//		// --- 2nd visit? ---
		//		if ((*curr).isVisited()) { // Implies curr is a leaf()
		//
		//			curr.expand();
		//
		//			if (curr.hasChildren()) {
		//				MCTS_Node::iterator parent = curr;
		//
		//				curr.toFirstChild();
		//
		//				// TODO: Optimization: Since we will "eventually" evaluate all children 
		//				// we can optionally evaluate all children at once
		//				// without significantly changing the algorithms behavior.
		//				// This can be a good optimization when evaluations are 
		//				// more efficient in batches.
		//				bool maximizeWhite = (*curr).position().moveCounter().isBlacksTurn();
		//				eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
		//
		//				// *** We have completely discovered and evaluated a new node ***
		//				m_searchMonitor.nodeCount.add(1);	// Atomic: nodeCount++;
		//
		//				// --- Backpropagate One ---
		//				// Backpropagate just this node so that lurking threads will know that 
		//				// this node has been visited
		//				(*curr).update(eval);
		//				curr = parent;
		//				(*curr).update(eval);
		//				(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//			}
		//			else {
		//				// *** Terminal Node ***
		//				bool maximizeWhite = (*curr).position().moveCounter().isBlacksTurn();
		//				GameState gstate;
		//				gstate(*curr);
		//				eval = 1'500 * gstate.getValue(maximizeWhite);	// count a win as 15 pawns
		//
		//				(*curr).lastVisit();
		//				(*curr).update(eval);
		//				(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//			}
		//		} // end if ((*curr).isVisited())
		//		//			// --- 1st visit?  ---
		//		//			else {
		//		//				eval = this->m_heuristicPtr->eval((*curr).position());
		//		//
		//		//				// *** We have completely discovered and evaluated a new node ***
		//		//				m_searchMonitor.nodeCount.add(1);	// Atomic: nodeCount++;
		//		//
		//		//				(*curr).update(eval);	// Indicates that this node has now been visited once
		//		//				(*curr).mutex().unlock(); // ~~~~~~~~~~~~~~~~ Unlock ~~
		//		//				curr.toParent();
		//		//			}
		//		//		}
		//		//		else {
		//		//			// --- Selection ---
		//		// Go back to root and start search again.
		//		//			curr = m_nodeTree.root();
		//		//			
		//		//			if (m_searchMonitor.exitConditionReached()) {
		//		//				// ~~~ Its possible to overstep the node count and ~~~ 
		//		//				// ~~~ search depth limit even with atomics. But ~~~
		//		//				// ~~~ that is not bad ~~~
		//		//				break;
		//		//			}
		//		//			else {
		//		//				continue;	// continue to top of loop 
		//		//			}
		//		//		}
		//		//		
		//		//		// --- Backpropagate ---
		//		//		while (curr.isRoot() == false) {
		//		//			(*curr).mutex().lock();
		//		//			(*curr).update(eval);			// !!! CS
		//		//			(*curr).mutex().unlock();
		//		//			curr.toParent();				// !!! Must be outside CS to unlock properly
		//		//		}
		//		//		
		//		//		// *** Now curr is at the root ***
		//		//		(*curr).mutex().lock();
		//		//		(*curr).update(eval);	// one more time for the root	// !!! CS 
		//		//		(*curr).mutex().unlock();
		//		//		
		//		//		// --- Check stopping condition ---
		//		//		if (m_searchMonitor.exitConditionReached()) {
		//		//			// ~~~ Its possible to overstep the node count and ~~~ 
		//		//			// ~~~ search depth limit even with atomics. But ~~~
		//		//			// ~~~ that is not bad ~~~
		//		//			break;
		//		//		}
		//	} // end if (mutex.try_lock())
		//} // end else /* non-leaf */
	    //} // end while (true)
	} // end solveOneThread()

	void MCTS_Concurrent::solve() {
		// --- Start ---
		m_nodeTree.root().expand();

		// ~~~ Concurrent Section ~~~

		std::vector<boost::thread> pool;
		const size_t nThreads = (m_nThreads == 0 ? boost::thread::hardware_concurrency() : m_nThreads);
		pool.reserve(nThreads);

		for (size_t t = 0; t < nThreads; t++) {
			pool.push_back(
				boost::thread{
					[this]() { this->solveOneThread(); }
				}
			);
		}

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		for (boost::thread& t : pool) {
			t.join();
		}

		// ~~~ End of Concurrent Section ~~~
	}
} // namespace forge