#pragma once
#include "Node.h"

namespace forge
{
	class MiniMaxNode : public Node
	{
	public:		// ---------- METHODS -----------------------------------------
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

		MiniMaxNode& parent() { return *m_parentPtr; }
		const MiniMaxNode& parent() const { return *m_parentPtr; }

		heuristic_t& fitness() { return m_fitness; }
		const heuristic_t& fitness() const { return m_fitness; }

		// TODO: Make it std::shared_ptr<const MiniMaxNode>
		const std::shared_ptr<MiniMaxNode>& bestMovePtr() const { return m_bestChildPtr; }
		///MiniMaxNode & bestMove() { return *m_bestChildPtr; }

		//std::vector<std::shared_ptr<MiniMaxNode>> & children() { return m_childrenPtrs; }
		const std::vector<std::shared_ptr<MiniMaxNode>>& children() const { return m_childrenPtrs; }

	private:	// ---------- FIELDS ------------------------------------------
		// Address of next child node if one exists
		// otherwise address of parent
		// Do not deallocate
		MiniMaxNode* m_nextPtr = nullptr;

		// Address of parent node
		// if nullptr then this object is the root of the tree
		// Do not deallocate
		MiniMaxNode* m_parentPtr = nullptr;

		// Fitness of this position based on a minimax of children
		heuristic_t m_fitness = 0;

		// Best move cooresponding to child with the best fitness
		// TODO: Make it std::shared_ptr<const MiniMaxNode>
		std::shared_ptr<MiniMaxNode> m_bestChildPtr = nullptr;
		//const MiniMaxNode * m_bestChildPtr = nullptr;	// Move m_bestMove;

		// Addresses of children nodes
		// TODO: Why not store children as a vector<MiniMaxNode> instead?
		// TODO: Whould it make sorting slower?
		std::vector<std::shared_ptr<MiniMaxNode>> m_childrenPtrs;

		// Set to PRUNED when all children have been fully searched and pruned.
		// Set from the pruneChildren() method
		enum class STATE : uint8_t {
			FRESH,		// Node has been created but hasn't been expanded yet (children have not been generated)
			EXPANDED,	// Node has been expanded so it may have children (children have been generated)
			PRUNED,		// Node has been fully searched and its children pruned (no need to search this node anymore)
		} m_state = STATE::FRESH;

	public:		// ---------- ITERATOR ----------------------------------------
		class iterator {
		public:
			iterator(MiniMaxNode* nodePtr) : p_node(nodePtr) {}
			iterator(MiniMaxNode* nodePtr, int depth, int depthLimit) :
				p_node(nodePtr),
				depth(depth),
				depthLimit(depthLimit) {}
			iterator& operator++();
			iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
			bool operator==(const iterator& it) const { return this->p_node == it.p_node; }
			bool operator!=(const iterator& it) const { return this->p_node != it.p_node; }
			MiniMaxNode& operator*();
			
			void setDepthLimit(int nPlys) { depthLimit = nPlys; }
			int getDepth() const { return depth; }
			bool isLeafNode() const { return depth >= depthLimit; }
			size_t getNodeCount() const { return m_nodeCount; }

		protected:
			bool nextSiblingExists() const { return p_node->m_nextPtr != nullptr; }
			bool parentExists() const { return p_node->m_parentPtr != nullptr; }
			bool firstChildExists() const { return p_node->m_childrenPtrs.size(); }

			void goToNextSibling()
			{
				p_node = p_node->m_nextPtr;	// go to next sibling
				m_nodeCount++;			// We've reached a new node
			}

			void goToParent()
			{
				p_node = p_node->m_parentPtr;	// go to parent (might be nullptr)
				depth--;				// going up one ply

				if (p_node != nullptr) {
					// *** Now we're at the parent node ***
					p_node->prune();			// prune children of parent

					// We've already been here so we need to keep iterating to 
					// later nodes
					++(*this);
				}
			}

			void goToFirstChild()
			{
				// One or more children.
				p_node = p_node->children().front().get();	// Go the 1st child.
				depth++;									// going down one ply
				m_nodeCount++;								// We've reached a new node
			}

		private:
			MiniMaxNode* p_node = nullptr;
			
			int depth = 0;

			int depthLimit = std::numeric_limits<int>::max();	// limitless 

			// Counts the number of nodes that this iterator has reached counting each node once.
			size_t m_nodeCount = 0;
		}; // end class iterator

		iterator begin() { return iterator(this); }
		iterator end() { return iterator(nullptr); }	// TODO: do we need this and does it make sense
	}; // end class MiniMaxNode
} // namespace forge