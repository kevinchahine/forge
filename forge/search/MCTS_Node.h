#pragma once

#include <forge/core/Node.h>

#include <limits>
#include <cmath>

namespace forge {
	class MCTS_Node : public NodeTemplate<MCTS_Node>
	{
	public:
		using super_t = NodeTemplate<MCTS_Node>;

		// UCB = x_i + C * sqrt(ln(N) / n_i)
		// 	x_i - average value of game state (t / n)
		// 	N - Parent node visits
		// 	n_i - Current node visits(if n_i is 0 then use 1 / inf to avoid division by zero)
		static float calcUCB(float average, float parentVisits, float currVisits) {
			// UCB = x_i + C * sqrt(ln(N) / n_i)
			// !!! Warning: parentVisits and currVisits must be greater than 0.
			//	log(0) = -NaN (undefined)
			//	x/0	= NaN (undefined)
			return average + MCTS_Node::temperature * sqrt(log(parentVisits) / currVisits);
		}

		int totalScore() const { return static_cast<int>(t); }

		int nGamesVisited() const { return static_cast<int>(n); }

		bool isVisited() const { return static_cast<int>(n) == 0; }
		bool isUnvisited() const { return !isVisited(); }

		float average() const { return t / n; }

		float ucb() const;

		// Warning: Update should never be called on the root node. 
		// Call updateRoot() instead
		void update(int score);

		// Accumulates t and n from 'node'
		// ucb and temperature are left unchanged
		// ucb will need to be recalculated after calling this method if node.t or node.n are non-zero
		void merge(const MCTS_Node& node);

	private: // -------------------------- PRIVATE FIELDS ------------------------
		// Total score of all children
		// # of games won by white
		float t = 0.0f;

		// Total number of games visited
		// Positive means white is winning
		// Negative means black is winning
		// Zero     means both are drawing
		// Set to positive epsilon
		// The root node must initialize this to 1.0f to prevent undefined ucb calculations.
		// Do this by calling MCTS_Node::updateRoot(0.0f) once when root node is initialized.
		// Hint: 
		//	log(0.0<x<1.0) < 0.0
		//	sqrt(-1) is undefined
		float n = std::numeric_limits<float>::min();

		static const float temperature;

	public: // ---------------------------- ITERATOR -----------------------------
		class iterator {
		public:
			iterator() = default;
			iterator(MCTS_Node* nodePtr) : p_node(nodePtr) {}
			iterator(MCTS_Node* nodePtr, int depth, int depthLimit) : p_node(nodePtr) {}
			bool operator==(const iterator& it) const { return this->p_node == it.p_node; }
			bool operator!=(const iterator& it) const { return this->p_node != it.p_node; }
			MCTS_Node& operator*();

			bool hasParent() const { return p_node->m_parentPtr != nullptr; }
			bool hasChildren() const { return p_node->m_childrenPtrs.size(); }
			bool isRoot() const { return hasParent() == false; }

			void toParent()
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
			void toSelectedChild(bool maximize);

			// Selects child with the best average value.
			// Then moves to that child.
			// maximize = true:  look for max ucb. favors white player
			// maximize = false: look for min ucb. favors black player
			// * See comments for goToSelectedChild()
			void toBestChild(bool maximize);

			// * See comments of goToSelectedChild()
			void toFirstChild();

		private:
			MCTS_Node* p_node = nullptr;
		}; // end class iterator

		iterator root() { return iterator(this); }
		iterator begin() { return iterator(this); }
		iterator end() { return iterator(nullptr); }	// TODO: do we need this and does it make sense
	}; // end class MCTS_Node
} // namespace forge