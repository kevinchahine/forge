#pragma once
#include "Node.h"

namespace forge {
    class MCTS_Node : public Node
    {
    public:		// ---------- METHODS -----------------------------------------
		void reset();

		// Takes the Position of this Node and generates children nodes using a valid move generator
		// The Children nodes can be accessed using the .children() method.
		// Should not be called on a Node with existing children.
		// If called on a Node with existing children, the original children will be deleted
		// and replace by the new ones.
		void expand();

		MCTS_Node& parent() { return *m_parentPtr; }
		const MCTS_Node& parent() const { return *m_parentPtr; }

		const std::vector<std::shared_ptr<MCTS_Node>>& children() const { return m_childrenPtrs; }

    private:	// ---------- FIELDS ------------------------------------------
        // Address of parent node
        // if nullptr then this object is the root of the tree
        // Do not deallocate
        MCTS_Node* m_parentPtr = nullptr;

        // Addresses of children nodes
        std::vector<std::shared_ptr<MCTS_Node>> m_childrenPtrs;

    public:		// ---------- ITERATOR ----------------------------------------
        class iterator {
        public:
			iterator(MCTS_Node* nodePtr) : p_node(nodePtr) {}
			iterator(MCTS_Node* nodePtr, int depth, int depthLimit) :
				p_node(nodePtr) {}
			iterator& operator++();
			iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
			bool operator==(const iterator& it) const { return this->p_node == it.p_node; }
			bool operator!=(const iterator& it) const { return this->p_node != it.p_node; }
			MCTS_Node& operator*();

		protected:
			bool parentExists() const { return p_node->m_parentPtr != nullptr; }
			bool firstChildExists() const { return p_node->m_childrenPtrs.size(); }

			void goToParent()
			{
				//p_node = p_node->m_parentPtr;	// go to parent (might be nullptr)
				//
				//if (p_node != nullptr) {
				//	// *** Now we're at the parent node ***
				//
				//	// We've already been here so we need to keep iterating to 
				//	// later nodes
				//	++(*this);
				//}
			}

			void goToFirstChild()
			{
				//// One or more children.
				//p_node = p_node->children().front().get();	// Go the 1st child.
			}

		private:
			MCTS_Node* p_node = nullptr;
		}; // end class iterator

		iterator begin() { return iterator(this); }
		iterator end() { return iterator(nullptr); }	// TODO: do we need this and does it make sense
    }; // end class MCTS_Node
} // namespace forge