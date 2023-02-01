#include "MCTS_ProducerConsumer.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class
#include <boost/lockfree/queue.hpp>

using namespace std;

namespace forge
{
	void search(MCTS_Node::iterator it) {

	}
//	void MCTS_ProducerConsumer::select(WorkQueue& evalExpandWork)
//	{
//		//selectionQueue.push(nodes);
//
//		// Mark nodes as already being selected
//	}
//	
//	void MCTS_ProducerConsumer::expandAndEvaluate(WorkQueue& evalExpandWork, WorkQueue& backpropWork)
//	{
//		while (true) {
//
//			MCTS_Node::iterator it;
//			
//			/////evalExpandWork.pop(it);
//
//			// -- -Expand or Evaluate ---
//			if ((*it).nVisits() == 0) {
//				// Evaluate
//				heuristic_t eval = this->m_heuristicPtr->eval((*it).position());
//			}
//			else if ((*it).nVisits() == 1) {
//				// Expand then Evaluate 1st Child
//				(*it).expand();
//
//				it.toFirstChild();
//
//				heuristic_t eval = this->m_heuristicPtr->eval((*it).position());
//			}
//			else {
//				// Error:
//				cout << "Error: " << __FILE__ << " line " << __LINE__ << " this node has already been evaluated." << endl;
//			}
//
//			// TODO: Mark nodes as evaluated (thats nvisites)
//
//			/////backpropWork.push(it);
//
//			break;// todo: REMOVE THIS
//		}
//	}
//	
//	void MCTS_ProducerConsumer::backpropagate(WorkQueue& backpropWork)
//	{
//		while (true) {
//			MCTS_Node::iterator it;
//
//			/////backpropWork.pop(it);
//
//			int score = (*it).totalScore();
//
//			while (it.isRoot() == false) {
//				// TODO: synchronize
//				(*it).update(score);
//
//				it.toParent();
//			}
//
//			break;
//		}
//	}

	MovePositionPair MCTS_ProducerConsumer::solve(const Position & position) {
		auto & sm = m_searchMonitor;

		WorkQueueA workA;
		WorkQueueB workB;

		boost::atomic_int workASize = 0;

		// --- Spawn Worker Threads ---
		vector<boost::thread> pool;
		pool.reserve(m_nThreads);

		for (size_t t = 0; t < m_nThreads; t++) {
			MCTS_Node::iterator it;
			pool.emplace_back([&]() { search(it); });
		}

		while (!sm.exitConditionReached()) {
			// --- 1.) Select Leaf Nodes (Produce) ---
			if (workASize < m_nThreads) {
				MCTS_Node::iterator leaf; // = selectNextBest();

				workA.push(leaf);

				workASize++;
			}

			// --- 2.) Back Propagate (Consume) ---
			NodeItEvalVisits nev;

			bool isPopped = workB.pop(nev);

			if (isPopped) {
				// Call back propagate
				// backPropagate(nev);
			}

		} // while(

		// --- Join Threads ---
		for (boost::thread & t : pool) {
			t.join();
		}

		return MovePositionPair();
	}

	//MovePositionPair MCTS_ProducerConsumer::solve(const Position& position)
	//{
	//	// // Selector finds leaf nodes, then puts them in this work queue.
	//	// // Other threads then expand and eval the leaf nodes.
	//	// WorkQueue evalExpandWork;
	//	// 
	//	// // When threads evaluate leaf nodes, they put those nodes in this work queue.
	//	// // The back propagation thread works on this work queue.
	//	// WorkQueue backpropWork;
	//	// 
	//	// // --- Spawn Selection Threads (Producers) ---
	//	// boost::thread selector = boost::thread{
	//	// 	[&]() { select(evalExpandWork); }
	//	// };
	//	// 
	//	// // --- Spawn Expand and Evaluate Threads (Producers/Consumers) ---
	//	// const size_t N_THREADS = boost::thread::hardware_concurrency();
	//	// 
	//	// vector<boost::thread> expanderPool;
	//	// 
	//	// for (size_t t = 0; t < N_THREADS - 2; t++) {
	//	// 	expanderPool.emplace_back(
	//	// 		[&]() { expandAndEvaluate(evalExpandWork, backpropWork); }
	//	// 	);
	//	// }
	//	// 
	//	// // --- Spawn Back Propagation Threads (Consumers) ---
	//	// boost::thread backpropagater = boost::thread{
	//	// 	[&]() { backpropagate(backpropWork); }
	//	// };
	//	// 
	//	// // --- Join Threads ---
	//	// 
	//	// selector.join();
	//	// 
	//	// for (size_t t = 0; t < expanderPool.size(); t++) {
	//	// 	expanderPool.at(t).join();
	//	// }
	//	// 
	//	// backpropagater.join();
	//	// 
	//	// // *** TODO: Optimize: let main thread do backpropagation. Then we will have one less thread to spawn.
	//
	//	return selectBestMove();
	//}
} // namespace forge

