#pragma once

#include <forge/core/Node.h>
#include <forge/search/UCB.h>

#include <limits>
#include <cmath>
#include <stack>
#include <vector>

#include <boost/thread/mutex.hpp>

namespace forge
{
	class MCTS_Node : public NodeTemplate<MCTS_Node>
	{
	public:
		using super_t = NodeTemplate<MCTS_Node>;

		int totalScore() const { return static_cast<int>(t); }

		int nVisits() const { return static_cast<int>(n); }

		bool isVisited() const { return n >= 0.9f; }
		bool isUnvisited() const { return !isVisited(); }

		// Puts node in a state that it will not be selected by UCB score.
		// Sets n visits to infinity.
		void lastVisit();

		float average() const { return t / n; }

		float ucb() const { return ucbScore; }

		void setFlag() { flag = true; }
		void clearFlag() { flag = false; }
		bool flagIsSet() const { return flag == true; }
		bool flagIsCleared() const { return flag == false; }

		void expand();

		// Adds score to total score
		// Increments number of visits
		// Warning: Do not call this method on a root node (node that has no parent)
		// Instead call updateRoot() See below
		void update(float score, float nEvals = 1);

		// Adds score to total score
		// Increments number of visits
		// This is intended for updating a root node (a node with no parents)
		void updateRoot(float score, float nEvals = 1);

		// Accumulates t and n from 'node'
		// ucb and temperature are left unchanged
		// ucb will need to be recalculated after calling this method if node.t or node.n are non-zero
		void merge(const MCTS_Node & node);

		// Sorts children pointers according to UCB scores.
		// Should be called once after each update.
		// Does not need to be called after update() if the order doesn't change.
		// Performs an insertion sort which works faster when elements are already mostly in order.
		void sort();

		// Get the Positions of children that this iterator currently points to.
		// Stores positions as a vector of pointers
		std::vector<const Position *> getChildrenPositions();

		float updateChildrenUCB(const std::vector<heuristic_t> & childrenEvals);

	private: // -------------------------- PRIVATE FIELDS ------------------------
		// Total score of all children
		// # of games won by white - 
		// # of games won by black
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

		// Set this value to -inf to identify a node as one which should not be selected during
		// the selection phase. 
		// ex: terminal nodes should be marked with -inf by calling this->lastVisit()
		float mark = 0.0f;

		// true - this node is being searched by some thread
		// false - else
		bool flag = false;

		static const float temperature;

		// Stored here to prevent recalculating between updates.
		float ucbScore = 0.0f;

	public: // ---------------------------- ITERATOR -----------------------------
		class iterator
		{
		public:
			iterator() = default;
			iterator(const iterator &) = default;
			iterator(iterator &&) noexcept = default;
			iterator(MCTS_Node * nodePtr) : p_node(nodePtr) {}
			~iterator() noexcept = default;
			iterator & operator=(const iterator &) = default;
			iterator & operator=(iterator &&) noexcept = default;

			bool operator==(const iterator & it) const { return this->p_node == it.p_node; }
			bool operator!=(const iterator & it) const { return this->p_node != it.p_node; }
			const MCTS_Node & operator*() const { return *p_node; }
			MCTS_Node & operator*() { return *p_node; }

			bool isExpanded() const { return p_node->isExpanded(); }
			bool hasParent() const { return p_node->m_parentPtr != nullptr; }
			bool hasChildren() const { return p_node->m_childrenPtrs.size(); }
			bool isRoot() const { return p_node->isRoot(); }
			bool isLeaf() const { return p_node->isLeaf(); }

			void expand() { p_node->expand(); }

			// Moves iterator to parent node
			// Can move to a null parent.
			// If called on root, this iterator will be invalidated.
			void toParent() {
				p_node = p_node->parentPtr();
			}

			// Selects the child with the highest UCB value
			// Then moves to that child.
			// Warning: Before calling this method, make sure that the current node
			// has been expanded and also has children.
			// An error will occur, if this method is called on a node without
			// any children.
			// maximize - determines whether the selection should favor children which higher
			//				or lower UCB scores. if maximize == true, then method favors higher UCB scores
			void toBestUCB(int nThBest = 0);

			// Selects child with the best average value.
			// Then moves to that child.
			// maximize = true:  look for max ucb. favors white player
			// maximize = false: look for min ucb. favors black player
			// * See comments for goToSelectedChild()
			void toBestAverage();

			void toMostVisited();

			// * See comments of goToSelectedChild()
			void toFirstChild();

			void toBestStochastic();

			bool isNull() const { return p_node == nullptr; }
			bool isNotNull() const { return !isNull(); }

		private:
			// node which iterator is currently referencing
			//std::stack<std::vector<std::shared_ptr<MCTS_Node> nodes;
			MCTS_Node * p_node = nullptr;
		}; // end class iterator

		iterator root() { return iterator(this); }
		iterator parent() { return iterator(this->m_parentPtr); }

	}; // end class MCTS_Node
} // namespace forge