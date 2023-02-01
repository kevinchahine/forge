#include "MCTS_Concurrent.h"

#include <forge/core/GameState.h>

#include <thread>

using namespace std;

namespace forge
{
	void MCTS_Concurrent::solve() {}
	//void MCTS_Concurrent::solveOneThread() {
	//	// --- Start ---
	//	MCTS_Node::iterator curr = m_nodeTree.root();
	//	auto & sm = m_searchMonitor;
	//
	//	while (true) {
	//		float eval = 0.0f;
	//		size_t nEvals = 0;
	//
	//		// --- Is Expanded? ---
	//		if (curr.isExpanded()) {
	//			// Yes node has been Expanded. Continue with selection.
	//
	//			// --- Selection ---
	//
	//			// --- Move DOWN the tree ---
	//			if (curr.hasChildren()) {
	//				curr.toBestUCB();
	//			}
	//			else {
	//				cout << "Error: bad re-visit" << endl;
	//				// We reached a terminal node (expended but without children).
	//				// The only thing we can do here is go back to the root and continue the search.
	//				(*curr).nBadVisits++;	// TODO: remove
	//
	//				curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.
	//
	//				if (sm.exitConditionReached()) {
	//					break;
	//				}
	//			}
	//		}
	//		else {
	//			// --- Expand ---
	//			if ((*curr).mutex().try_lock()) {
	//				// No node is still Fresh
	//				curr.expand();
	//				
	//				// --- Is Terminal or Intermediate? ---
	//				if (curr.hasChildren()) {
	//					// Intermediate Node. Evaluate using Heuristic.
	//
	//					bool maximizeWhite = (*curr).position().isWhitesTurn();
	//
	//					vector<const Position *> pChildren = (*curr).getChildrenPositions();
	//
	//					vector<heuristic_t> evals = this->m_heuristicPtr->eval(pChildren, maximizeWhite);
	//
	//					eval = -(*curr).updateChildrenUCB(evals);
	//
	//					nEvals = evals.size();
	//				}
	//				else {
	//					// Terminal Node. Evaluate using Game State.
	//					bool maximizeWhite = (*curr).position().isBlacksTurn();
	//					GameState gstate;
	//					gstate.init(*curr);
	//					eval = (float) UCB::WINNING_EVAL * gstate.getValue(maximizeWhite);	// count a win as 15 pawns
	//					nEvals = 1;
	//
	//					(*curr).lastVisit();
	//				}
	//			}
	//			else {
	//				// Back to root
	//				curr = m_nodeTree.root();
	//				continue;
	//			}
	//
	//			(*curr).mutex().unlock();
	//
	//			// --- Backpropagate ---
	//			while (curr.isRoot() == false) {
	//				(*curr).update(eval);
	//				eval = -eval;
	//				curr.toParent();
	//				(*curr).sort();
	//			}
	//
	//			//curr = m_nodeTree.root();
	//
	//			// *** Now curr is at the root ***
	//
	//			// --- Check stopping condition ---
	//			sm.nodeCount.add(nEvals);
	//			if (sm.exitConditionReached()) {
	//				sm.stop();	// stop the clock so we can record exact search time.
	//				break;
	//			}
	//		}
	//	} // end white (true)
	//} // end solveOneThread()
	//
	//void MCTS_Concurrent::solve() {
	//	// --- Start ---
	//	m_nodeTree.root().expand();
	//
	//	// ~~~ Concurrent Section ~~~
	//
	//	std::vector<thread> pool;
	//	const size_t nThreads = (m_nThreads == 0 ? thread::hardware_concurrency() : m_nThreads);
	//	pool.reserve(nThreads);
	//
	//	for (size_t t = 0; t < nThreads; t++) {
	//		pool.push_back(
	//			thread{
	//				[this]() { this->solveOneThread(); }
	//			}
	//		);
	//	}
	//
	//	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//	for (thread & t : pool) {
	//		t.join();
	//	}
	//
	//	// ~~~ End of Concurrent Section ~~~
	//}
} // namespace forge

void oldSolveOneThread() {
	//// --- Is node a leaf? ---
			//// *** Leaves are nodes which have not yet been expanded ***
			//if (curr.isLeaf()) {
			//	// *** Curr is pointing to a LEAF node ***
			//	heuristic_t eval = 0;
			//
			//	// ~~~ Attempt to Aquire the Lock ~~~
			//	//while ((*curr).mutex().try_lock() == false) {
			//	//	curr.toNext();
			//	//}
			//	(*curr).mutex().lock();
			//
			//	// ~~~ Here we have aquired the lock on node `curr` ~~~
			//
			//	// --- 1st Visit? ---
			//	if ((*curr).nVisits() == 0) {
			//		// --- Evaluate ---
			//		bool maximizeWhite = (*curr).position().isBlacksTurn();
			//		eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
			//		(*curr).update(eval);
			//		eval = -eval;
			//	}
			//	// --- 2nd Visit? ---
			//	else if ((*curr).nVisits() == 1) {
			//		// --- Expand ---
			//		(*curr).m_expandMutex.lock();
			//		curr.expand();
			//		(*curr).m_expandMutex.unlock();
			//
			//		// --- Evaluate ---
			//		if (curr.hasChildren()) {
			//			// *** Intermediate Node ***
			//
			//			curr.toFirstChild(); // Down 1
			//
			//			bool maximizeWhite = (*curr).position().isBlacksTurn();
			//			eval = this->m_heuristicPtr->eval((*curr).position(), maximizeWhite);
			//			(*curr).update(eval);
			//			eval = -eval;
			//
			//			curr.toParent(); // Up 1
			//		}
			//		else {
			//			// *** Terminal Node ***
			//			bool maximizeWhite = (*curr).position().isBlacksTurn();
			//			GameState gstate;
			//			gstate(*curr);
			//
			//			eval = 1500 * gstate.getValue(maximizeWhite);
			//			(*curr).update(eval);
			//			eval = -eval;
			//			(*curr).lastVisit();
			//		}
			//	}
			//	else {
			//		// TODO: Optimize: See how frequently this occures. 
			//		// If it is frequent enough, then fix it by having curr
			//		// move up one node and continueing the loop instead. 
			//		// Don't forget to unlock first.
			//		(*curr).mutex().unlock();
			//		curr = m_nodeTree.root();// TODO: Instead check stopping condition and continue.
			//		continue;
			//	}
			//
			//	// ~~~ Release the Lock ~~~
			//	(*curr).mutex().unlock();
			//
			//	curr.toParent();
			//	(*curr).mutex().lock();
			//	(*curr).sort();
			//	(*curr).mutex().unlock();
			//
			//	// --- BackPropagate ---
			//	while (curr.hasParent()) {
			//		// TODO: WRITE lock goes here
			//		(*curr).mutex().lock();
			//		(*curr).update(eval);
			//		eval = -eval;
			//		(*curr).mutex().unlock();
			//
			//		curr.toParent();
			//		(*curr).mutex().lock();
			//		(*curr).sort();
			//		(*curr).mutex().unlock();
			//	}
			//
			//	curr = m_nodeTree.root();// *** Now curr is at the root ***
			//
			//	// --- Check stopping condition ---
			//	sm.nodeCount++;
			//	if (sm.exitConditionReached()) { break; }
			//} // end if (curr.isLeaf())
			//else {
			//	// *** Curr is pointing to an INTERMEDIATE node ***
			//	// --- Move DOWN the tree ---
			//	if ((*curr).isPruned() == false && curr.hasChildren()) {
			//		// TODO: READ LOCK GOES HERE
			//		MCTS_Node::iterator prev = curr;
			//		(*prev).mutex().lock();
			//		curr.toBestUCB();
			//		(*prev).mutex().unlock();
			//	}
			//	else {
			//		cout << guten::color::lightred << "Bad Visit!!!" << guten::color::white << endl;
			//		// We reached a terminal node (expended but without children).
			//		// The only thing we can do here is go back to the root and continue the search.
			//		(*curr).nBadVisits++;	// TODO: remove
			//		//badTraversals++;		// TODO: remove. remove field from class also
			//
			//		curr = m_nodeTree.root();	// Causes a deadlock without checking the exit condition.
			//
			//		if (sm.exitConditionReached()) {
			//			break;
			//		}
			//	}
			//}
}