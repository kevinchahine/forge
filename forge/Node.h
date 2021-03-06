#pragma once

#include "Position.h"

#include <vector>
#include <memory>
#include <mutex>

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
	//	and all its children without having to communicate with other threads.
	//	Search algorithms that use a queue or stack
	//	will need a mutex that is constantly locked and unlocked as threads access it greately
	//	increasing overhead as more and more threads are used.
	class Node
	{
	public:
		// Takes the board of this Node and generates children nodes using a valid move generator
		// The Children nodes can be accessed using the .children() method.
		// If called on a Node with existing children, the original children will be deleted
		// and replace by the new ones.
		void expand();

		std::vector<std::unique_ptr<Node>> & children() { return m_childrenPtrs; }
		const std::vector<std::unique_ptr<Node>> & children() const { return m_childrenPtrs; }

	private:
		// Keep this here for later.
		std::mutex m_lock;

		// Stores a position of the game
		Position m_position;

		// Address of parent node
		// if nullptr then this object is the root of the tree
		// No need to deallocate
		Node * m_parentPtr = nullptr;
		
		// Address of next child node
		// move efficient than using an tree iterator
		// No need to deallocate
		Node * m_siblingPtr = nullptr;

		// Addresses of children nodes
		std::vector<std::unique_ptr<Node>> m_childrenPtrs;
	};
} // namespace forge