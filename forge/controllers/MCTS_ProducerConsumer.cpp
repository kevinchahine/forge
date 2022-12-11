#include "MCTS_ProducerConsumer.h"

#include <forge/core/GameState.h>

#include <boost/thread/thread_only.hpp>		// for boost::thread class
#include <boost/lockfree/queue.hpp>

using namespace std;

namespace forge
{
	void MCTS_ProducerConsumer::reset()
	{
		m_nodeTree.reset();
	}

	MovePositionPair MCTS_ProducerConsumer::getMove(const Position& position)
	{
		return MovePositionPair();
	}
	
	MovePositionPair MCTS_ProducerConsumer::selectBestMove()
	{
		MCTS_Node::iterator bestIt = m_nodeTree.root();

		bool maximize = m_nodeTree.position().moveCounter().isWhitesTurn();

		//bestIt.toBestUCB(maximize);	// Stochastic selection
		//bestIt.toBestAverage(maximize);		// Best selection
		bestIt.toMostVisited();

		MovePositionPair solution{
			(*bestIt).move(),
			(*bestIt).position()
		};

		return solution;
	}
	
	void MCTS_ProducerConsumer::select(WorkQueue& selectionQueue)
	{
		//selectionQueue.push(nodes);

		// Mark nodes as already being selected
	}
	
	void MCTS_ProducerConsumer::expandAndEvaluate(WorkQueue& selectionQueue, WorkQueue& backPropQueue)
	{
		while (true) {

			MCTS_Node::iterator it;
			
			selectionQueue.pop(it);

			// -- -Expand or Evaluate ---
			if ((*it).nVisits() == 0) {
				// Evaluate
				heuristic_t eval = this->m_heuristicPtr->eval((*it).position());
			}
			else if ((*it).nVisits() == 1) {
				// Expand then Evaluate 1st Child
				(*it).expand();

				it.toFirstChild();

				heuristic_t eval = this->m_heuristicPtr->eval((*it).position());
			}
			else {
				// Error:
				cout << "Error: " << __FILE__ << " line " << __LINE__ << " this node has already been evaluated." << endl;
			}

			// TODO: Mark nodes as evaluated (thats nvisites)

			backPropQueue.push(it);

			break;// todo: REMOVE THIS
		}
	}
	
	void MCTS_ProducerConsumer::backpropagate(WorkQueue& backPropQueue)
	{
		while (true) {
			MCTS_Node::iterator it;

			backPropQueue.pop(it);

			int score = (*it).totalScore();

			while (it.isRoot() == false) {
				// TODO: synchronize
				(*it).update(score);

				it.toParent();
			}

			break;
		}
	}
	
	MovePositionPair MCTS_ProducerConsumer::solve(const Position& position)
	{
		// Selector finds leaf nodes, then puts them in this work queue.
		// Other threads then expand and eval the leaf nodes.
		WorkQueue expandEvalWork;

		// When threads evaluate leaf nodes, they put those nodes in this work queue.
		// The back propagation thread works on this work queue.
		WorkQueue backPropWork;

		// --- Spawn Selection Threads (Producers) ---
		boost::thread selector = boost::thread{
			[&]() { select(expandEvalWork); }
		};

		// --- Spawn Expand and Evaluate Threads (Producers/Consumers) ---
		const size_t N_THREADS = boost::thread::hardware_concurrency();

		vector<boost::thread> expanderPool;
		
		for (size_t t = 0; t < N_THREADS - 2; t++) {
			expanderPool.emplace_back([&]() { expandAndEvaluate(expandEvalWork, backPropWork); });
		}

		// --- Spawn Back Propagation Threads (Consumers) ---
		boost::thread backpropagater = boost::thread{
			[&]() { backpropagate(backPropWork); }
		};

		// --- Join Threads ---

		selector.join();

		for (size_t t = 0; t < expanderPool.size(); t++) {
			expanderPool.at(t).join();
		}
		
		backpropagater.join();

		// *** TODO: Optimize: let main thread do backpropagation. Then we will have one less thread to spawn.

		return MovePositionPair();
	}
} // namespace forge

