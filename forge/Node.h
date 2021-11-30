#pragma once

#include "Position.h"
#include "HeuristicBase.h"	// for heuristic_t

#include <vector>
#include <memory>
#include <mutex>
#include <limits>

namespace forge
{
	// Part of a NodeTree.
	// Used in search algorithms to store a Position and branches to other Positions
	// that are to be searched.
	// Can also be used to calculate 50 move rule by traversing from a node to its parents.
	// Multi-threading:
	//	Some search algorithms like breadth-first search and depth-first search use queues and
	//	stacks to store their frontiers. A Node Tree can be used instead to allow for more efficient 
	//	multi-threading. This is because threads can be assigned to search different 
	//	branches and will need little inter-thread interactions. Each thread can search a Node
	//	and allToFen its children without having to communicate with other threads.
	//	Search algorithms that use a queue or stack
	//	will need a mutex that is constantly locked and unlocked as threads access it greately
	//	increasing overhead as more and more threads are used.
	class Node
	{
	public:
		
		Move & move() { return m_move; }
		const Move & move() const { return m_move; }

		Position & position() { return m_position; }
		const Position & position() const { return m_position; }

	protected:
		// Keep this here for later.
		// TODO: Consider using shared_mutex so that when two or more threads are searching children
		//	one thread won't prune the other children.
		///std::mutex m_lock;

		// Stores the move that got us to this position from parent
		Move m_move;

		// Stores a position of the game
		Position m_position;
	};
} // namespace forge