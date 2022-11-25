#pragma once

#include <forge/core/Node.h>

#include <limits>
#include <cmath>

namespace forge {
	class MCTS_Node : public NodeTemplate<MCTS_Node>
	{
		using super_t = NodeTemplate<MCTS_Node>;
	
	public:		// ---------- METHODS -----------------------------------------
		// UCB = x_i + C * sqrt(ln(N) / n_i)
		// 	x_i - average value of game state (t / n)
		// 	C - constant "Temperature" (ex : 1.5)
		// 	N - Parent node visits
		// 	n_i - Current node visits(if n_i is 0 then use 1 / inf to avoid division by zero)
		static float calcUCB(float average, float temperature, int parentVisits, int currVisits);
		
		int totalScore() const { return static_cast<int>(t); }

		int nGamesVisited() const { return static_cast<int>(n); }

		float average() const { return t / n; }

		float ucb() const { return m_ucb; }
		
		// Warning: Update should never be called on the root node. 
		// Call updateRoot() instead
		void update(int rolloutResult);

		void updateRoot(int rolloutResult);

		// Accumulates t and n from 'node'
		// ucb and temperature are left unchanged
		// ucb will need to be recalculated after calling this method if node.t or node.n are non-zero
		void merge(const MCTS_Node& node);

	private:	// ---------- FIELDS ------------------------------------------
		// Total score of all children
		// # of games won by white
		float t = 0.0f;

		// Total number of games visited
		float n = std::numeric_limits<float>::min();

		// Save ucb here so that it doesn't need to be recalculated each time its used
		// Precalculate to max value to save time on construction
		// Its value is calculated from t n and temperature
		float m_ucb = std::numeric_limits<float>::max(); // 0.0f;

		float temperature = 1.5;	// TODO: Make this static const

	public:		// ---------- ITERATOR ----------------------------------------
		class iterator {
        public:
			iterator() = default;
			iterator(MCTS_Node* nodePtr) : p_node(nodePtr) {}
			iterator(MCTS_Node* nodePtr, int depth, int depthLimit) : p_node(nodePtr) {}
			iterator& operator++();
			iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
			bool operator==(const iterator& it) const { return this->p_node == it.p_node; }
			bool operator!=(const iterator& it) const { return this->p_node != it.p_node; }
			MCTS_Node& operator*();

			bool parentExists() const { return p_node->m_parentPtr != nullptr; }
			bool childrenExist() const { return p_node->m_childrenPtrs.size(); }
			bool isRoot() const { return parentExists() == false; }

			void goToParent()
			{
				p_node = p_node->m_parentPtr;	// go to parent (might be nullptr)
			}

			// Selects the child with the highest UCB value
			// Then moves to that child.
			// Warning: Before calling this method, make sure that the current node
			// has been expanded and also has children.
			// An error will occur, if this method is called on a node without
			// any children.
			// maximize - determines whether the selection should favor children which higher
			//				or lower UCB scores. if maximize == true, then method favors higher UCB scores
			void goToSelectedChild(bool maximize);

			// Selects child with the best average value.
			// Then moves to that child
			// * See comments for goToSelectedChild()
			void goToBestChild();

			// * See comments of goToSelectedChild()
			void goToFirstChild();

		private:
			MCTS_Node* p_node = nullptr;
		}; // end class iterator
		
		iterator begin() { return iterator(this); }
		iterator end() { return iterator(nullptr); }	// TODO: do we need this and does it make sense
    }; // end class MCTS_Node
} // namespace forge