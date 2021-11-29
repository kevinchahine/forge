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
		void reset();

		// Takes the Position of this Node and generates children nodes using a valid move generator
		// The Children nodes can be accessed using the .children() method.
		// Should not be called on a Node with existing children.
		// If called on a Node with existing children, the original children will be deleted
		// and replace by the new ones.
		void expand();

		// Prunes children. Deletes all their children.
		// opposite of expand()
		// Only necessary when saving memory.
		void prune();

		bool isFresh() const { return m_state == STATE::FRESH; }
		bool isExpanded() const { return m_state == STATE::EXPANDED; }
		bool isPruned() const { return m_state == STATE::PRUNED; }

		Move & move() { return m_move; }
		const Move & move() const { return m_move; }

		Position & position() { return m_position; }
		const Position & position() const { return m_position; }

		Node & parent() { return *m_parentPtr; }
		const Node & parent() const { return *m_parentPtr; }

		heuristic_t & fitness() { return m_fitness; }
		const heuristic_t & fitness() const { return m_fitness; }

		// TODO: Make it std::shared_ptr<const Node>
		const std::shared_ptr<Node> & bestMovePtr() const { return m_bestChildPtr; }
		///Node & bestMove() { return *m_bestChildPtr; }

		//std::vector<std::shared_ptr<Node>> & children() { return m_childrenPtrs; }
		const std::vector<std::shared_ptr<Node>> & children() const { return m_childrenPtrs; }

		// Designed to work with Minimax
		// For MTCS, we may need to implemente a different iterator.
		// see Node::prune()
		class iterator
		{
		public:

			iterator(Node * ptr) : ptr(ptr) {}
			iterator(Node * ptr, int depth, int depthLimit) :
				ptr(ptr),
				depth(depth),
				depthLimit(depthLimit) {}
			iterator& operator++();
			iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
			bool operator==(const iterator & it) const { return this->ptr == it.ptr; }
			bool operator!=(const iterator & it) const { return this->ptr != it.ptr; }
			Node & operator*();

			void setDepthLimit(int nPlys) { depthLimit = nPlys; }
			int getDepth() const { return depth; }
			bool isLeafNode() const { return depth >= depthLimit; }
			size_t getNodeCount() const { return m_nodeCount; }

		protected:
			bool nextSiblingExists() const { return ptr->m_nextPtr != nullptr; }
			bool parentExists() const { return ptr->m_parentPtr != nullptr; }
			bool firstChildExists() const { return ptr->m_childrenPtrs.size(); }

			void goToNextSibling()
			{
				ptr = ptr->m_nextPtr;	// go to next sibling
				m_nodeCount++;			// We've reached a new node
			}

			void goToParent()
			{
				ptr = ptr->m_parentPtr;	// go to parent (might be nullptr)
				depth--;				// going up one ply

				if (ptr != nullptr) {
					// *** Now we're at the parent node ***
					ptr->prune();			// prune children of parent
				
					// We've already been here so we need to keep iterating to 
					// later nodes
					++(*this);
				}
			}

			void goToFirstChild()
			{
				// One or more children.
				ptr = ptr->children().front().get();	// Go the 1st child.
				depth++;								// going down one ply
				m_nodeCount++;							// We've reached a new node
			}

		private:
			Node * ptr = nullptr;

			int depth = 0;
			
			int depthLimit = std::numeric_limits<int>::max();	// limitless 

			// Counts the number of nodes that this iterator has reached counting each node once.
			size_t m_nodeCount = 0;
		}; // end class iterator

		iterator begin() { return iterator(this); }
		iterator end() { return iterator(nullptr); }	// TODO: do we need this and does it make sense

	private:
		// Keep this here for later.
		// TODO: Consider using shared_mutex so that when two or more threads are searching children
		//	one thread won't prune the other children.
		///std::mutex m_lock;

		// Stores the move that got us to this position from parent
		Move m_move;

		// Stores a position of the game
		Position m_position;

		// Address of parent node
		// if nullptr then this object is the root of the tree
		// Do not deallocate
		Node * m_parentPtr = nullptr;
		
		// Address of next child node if one exists
		// otherwise address of parent
		// Do not deallocate
		Node * m_nextPtr = nullptr;

		// Fitness of this position based on a minimax of children
		heuristic_t m_fitness = 0;

		// Best move cooresponding to child with the best fitness
		// TODO: Make it std::shared_ptr<const Node>
		std::shared_ptr<Node> m_bestChildPtr = nullptr;
		//const Node * m_bestChildPtr = nullptr;	// Move m_bestMove;

		// Addresses of children nodes
		// TODO: Why not store children as a vector<Node> instead?
		// TODO: Whould it make sorting slower?
		std::vector<std::shared_ptr<Node>> m_childrenPtrs;

		// Set to PRUNED when all children have been fully searched and pruned.
		// Set from the pruneChildren() method
		enum class STATE : uint8_t {
			FRESH,		// Node has been created but hasn't been expanded yet (children have not been generated)
			EXPANDED,	// Node has been expanded so it may have children (children have been generated)
			PRUNED,		// Node has been fully searched and its children pruned (no need to search this node anymore)
		} m_state = STATE::FRESH;
	};
} // namespace forge