#include "MCTS_Concurrent.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class

using namespace std;

namespace forge
{
	void MCTS_Concurrent::solveOneThread() {
		// --- Start ---
		MCTS_Node::iterator curr = m_nodeTree.root();
		auto& sm = m_searchMonitor;

		while (true) {
			// --- Is node a leaf? ---
			// *** Leaves are nodes which have not yet been expanded ***
			if (curr.isLeaf()) {
				// *** Curr is pointing to a LEAF node ***
				heuristic_t eval = 0;

				// ~~~ Attempt to Aquire the Lock ~~~
				//while ((*curr).mutex().try_lock() == false) {
				//	curr.toNext();
				//}
				(*curr).mutex().lock();

				// ~~~ Here we have aquired the lock on node `curr` ~~~

				// --- 1st Visit? ---
				if ((*curr).nVisits() == 0) {
					// --- Evaluate ---
					bool maximizeWhite = (*curr).position().isBlacksTurn();
					eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
					(*curr).update(eval);
					eval = -eval;
				}
				// --- 2nd Visit? ---
				else if ((*curr).nVisits() == 1) {
					// --- Expand ---
					(*curr).m_expandMutex.lock();
					curr.expand();
					(*curr).m_expandMutex.unlock();

					// --- Evaluate ---
					if (curr.hasChildren()) {
						// *** Intermediate Node ***

						curr.toFirstChild(); // Down 1

						bool maximizeWhite = (*curr).position().isBlacksTurn();
						eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
						(*curr).update(eval);
						eval = -eval;

						curr.toParent(); // Up 1
					}
					else {
						// *** Terminal Node ***
						bool maximizeWhite = (*curr).position().isBlacksTurn();
						GameState gstate;
						gstate(*curr);

						eval = 1500 * gstate.getValue(maximizeWhite);
						(*curr).update(eval);
						eval = -eval;
						(*curr).lastVisit();
					}
				}
				else {
					// TODO: Optimize: See how frequently this occures. 
					// If it is frequent enough, then fix it by having curr
					// move up one node and continueing the loop instead. 
					// Don't forget to unlock first.
					(*curr).mutex().unlock();
					curr = m_nodeTree.root();// TODO: Instead check stopping condition and continue.
					continue;
				}

				// ~~~ Release the Lock ~~~
				(*curr).mutex().unlock();

				curr.toParent();
				(*curr).mutex().lock();
				(*curr).sort();
				(*curr).mutex().unlock();

				// --- BackPropagate ---
				while (curr.hasParent()) {
					// TODO: WRITE lock goes here
					(*curr).mutex().lock();
					(*curr).update(eval);
					eval = -eval;
					(*curr).mutex().unlock();

					curr.toParent();
					(*curr).mutex().lock();
					(*curr).sort();
					(*curr).mutex().unlock();
				}

				curr = m_nodeTree.root();// *** Now curr is at the root ***

				// --- Check stopping condition ---
				sm.nodeCount++;
				if (sm.exitConditionReached()) { break; }
			} // end if (curr.isLeaf())
			else {
				// *** Curr is pointing to an INTERMEDIATE node ***
				// --- Move DOWN the tree ---
				if ((*curr).isPruned() == false && curr.hasChildren()) {
					// TODO: READ LOCK GOES HERE
					MCTS_Node::iterator prev = curr;
					(*prev).mutex().lock();
					curr.toBestUCB();
					(*prev).mutex().unlock();
				}
				else {
					cout << guten::color::lightred << "Bad Visit!!!" << guten::color::white << endl;
					// We reached a terminal node (expended but without children).
					// The only thing we can do here is go back to the root and continue the search.
					(*curr).nBadVisits++;	// TODO: remove
					//badTraversals++;		// TODO: remove. remove field from class also

					curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.

					if (sm.exitConditionReached()) {
						break;
					}
				}
			}
		} // end white (true)
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