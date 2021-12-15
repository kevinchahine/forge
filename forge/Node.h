#pragma once

#include "Position.h"
#include "HeuristicBase.h"	// for heuristic_t
#include "MoveGenerator2.h"

#include <vector>
#include <memory>
#include <mutex>
#include <limits>
#include <iterator>

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
	template<class NODE_T>
	class NodeTemplate
	{
	public:
		void reset();

		void expand();
		
		void prune();

		Move & move() { return m_move; }
		const Move & move() const { return m_move; }

		Position & position() { return m_position; }
		const Position & position() const { return m_position; }

		bool isFresh() const { return m_state == STATE::FRESH; }
		bool isExpanded() const { return m_state == STATE::EXPANDED; }
		bool isPruned() const { return m_state == STATE::PRUNED; }

		// Root node is the top node of the tree.
		// It has no parent node.
		bool isRoot() const { return m_parentPtr == nullptr; }

		// Leaf nodes are node that do not have children yet. 
		// After being expanded, leaf nodes become:
		//	- intermediate nodes if they end up with children.
		//	- terminal ndoes if they do not.
		// Basically a leaf node is any node that hasn't been expanded.
		// A leaf node is not to be confused with a terminal node. 
		bool isLeaf() const { return m_state == STATE::FRESH; /* implies: children().empty(); */ }

		// Intermediate nodes are any nodes that have children.
		// Before a node is expanded it is a leaf node.
		// Warning: A node can only become intermediate after it has been expanded.
		//	If expanding a node creates no children, then the node is a terminal node meaning game over.
		bool isIntermediate() const { return m_state == STATE::EXPANDED && children().size(); }

		// Terminal nodes are any nodes that have been expanded but have no children.
		// After a node is expanded, if no children were generated, then we know
		// that no moves can be made from it. Its either a win, loss or draw.
		bool isTerminal() const { return m_state == STATE::EXPANDED && children().empty(); }

		NODE_T* parentPtr() { return m_parentPtr; }
		const NODE_T* parentPtr() const { return m_parentPtr; }

		std::vector<std::shared_ptr<NODE_T>> & children() { return m_childrenPtrs; }
		const std::vector<std::shared_ptr<NODE_T>> & children() const { return m_childrenPtrs; }

	protected:

		// Stores the move that got us to this position from parent
		Move m_move;

		// Stores a position of the game
		Position m_position;
			
		// Address of parent node
		// if nullptr then this object is the root of the tree
		// Do not deallocate
		NODE_T* m_parentPtr = nullptr;
		
		// Addresses of children nodes
		std::vector<std::shared_ptr<NODE_T>> m_childrenPtrs;
		
		// Set to PRUNED when all children have been fully searched and pruned.
		// Set from the pruneChildren() method
		enum class STATE : uint8_t {
			FRESH,		// Node has been created but hasn't been expanded yet (children have not been generated)
			EXPANDED,	// Node has been expanded so it may have children (children have been generated)
			PRUNED,		// Node has been fully searched and its children pruned (no need to search this node anymore)
		} m_state = STATE::FRESH;
	};	// class NodeTemplate<NODE_T>

	template<class NODE_T>
	void NodeTemplate<NODE_T>::reset()
	{
		// TODO: Code: If this method can be re-written to assign
		// a default object, then the derived classes would not need to 
		// implement their own reset() methods.
		// ex: (*this) = NodeTemplate<NODE_T>(); 
		// Should do the trick

		(*this) = NodeTemplate<NODE_T>{};	// default constructor

		// Old code
		//m_position.clear();
		//m_parentPtr = nullptr;
		//m_childrenPtrs.clear();
		//m_state = STATE::FRESH;
	}

	template<class NODE_T>
	void NodeTemplate<NODE_T>::expand()
	{
		MoveList moves;

		// 1.) --- Generate legal moves ---
		// Figures out wether white or black is playing and generates moves for them.
		MoveGenerator2 movegen;
		moves = movegen.generate(m_position);

		// 2.) --- Create children nodes ---
		m_childrenPtrs.clear();
		m_childrenPtrs.reserve(moves.size());

		for (const auto& move : moves) {
			auto sp = make_shared<NODE_T>();

			// -- Create new Node --
			m_childrenPtrs.emplace_back(std::make_shared<NODE_T>());

			// -- Alias --
			NODE_T& child = *m_childrenPtrs.back();

			// -- Assign proper values --
			child.m_move = move.move;
			child.m_position = move.position;	// TODO: Optimize: Slow copy
		}

		// 3.) --- Assign sibling pointers ---
		if (m_childrenPtrs.size()) {
			for (size_t i = 0; i < m_childrenPtrs.size() - 1; i++) {
				shared_ptr<NODE_T>& childPtr = m_childrenPtrs.at(i);
				
				childPtr->m_parentPtr = static_cast<NODE_T*>(this);
			}
		}

		// 4.) --- Assign last childs m_nextPtr to parent ---
		if (m_childrenPtrs.empty() == false) {
			m_childrenPtrs.back()->m_parentPtr = static_cast<NODE_T*>(this);
		}

		m_state = STATE::EXPANDED;
	}

	template<class NODE_T>
	void NodeTemplate<NODE_T>::prune()
	{
		m_childrenPtrs.clear();

		m_state = STATE::PRUNED;
	}
} // namespace forge

#include "Node.cpp"