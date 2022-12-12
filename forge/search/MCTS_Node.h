#pragma once

#include <forge/core/Node.h>

#include <limits>
#include <cmath>
#include <stack>

#include <boost/thread/mutex.hpp>

namespace forge {

	class MCTS_Node : public NodeTemplate<MCTS_Node>
	{
	public:
		using super_t = NodeTemplate<MCTS_Node>;

		// UCB = t/n + C * sqrt(ln(N) / n)
		// 	t - total score
		// 	N - Parent node visits
		// 	n - Current node visits
		// returns UCB score. Guarenteed to be a real number not +/-infinity or undefined.
		static float calcUCBWhite(float total, float parentVisits, float currVisits) {
			// UCB = x_i + C * sqrt(ln(N) / n_i)
			// !!! Warning: parentVisits and currVisits must be greater than 0.
			//	log(0) = -NaN (undefined)
			//	x/0	= NaN (undefined)
			return (total / (currVisits + 1.0f)) + MCTS_Node::temperature * sqrt(log(parentVisits + 1.0f) / (currVisits + 1.0f));
		}

		// UCB = t/n + C * sqrt(ln(N) / n)
		// 	t - total score
		// 	N - Parent node visits
		// 	n - Current node visits
		// returns UCB score. Guarenteed to be a real number not +/-infinity or undefined.
		static float calcUCBBlack(float total, float parentVisits, float currVisits) {
			// UCB = x_i + C * sqrt(ln(N) / n_i)
			// !!! Warning: parentVisits and currVisits must be greater than 0.
			//	log(0) = -NaN (undefined)
			//	x/0	= NaN (undefined)
			return (total / (currVisits + 1.0f)) - MCTS_Node::temperature * sqrt(log(parentVisits + 1.0f) / (currVisits + 1.0f));
		}

		int totalScore() const { return static_cast<int>(t); }

		int nVisits() const { return static_cast<int>(n); }

		bool isVisited() const { return static_cast<int>(n) == 0; }
		bool isUnvisited() const { return !isVisited(); }

		float average() const { return t / n; }

		//float ucb() const;
		float ucbWhite() const;
		float ucbBlack() const;

		boost::mutex& mutex() { return m_mutex; }
		const boost::mutex& mutex() const { return m_mutex; }

		// Adds score to total score
		// Increments number of visits
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

		// Used by multithreaded version of MCTS
		boost::mutex m_mutex;

	public: // ---------------------------- ITERATOR -----------------------------
		class iterator {
		public:
			iterator() = default;
			iterator(MCTS_Node* nodePtr) { nodes.push(nodePtr); }
			//iterator(MCTS_Node* nodePtr, int depth, int depthLimit) : p_node(nodePtr) {}
			bool operator==(const iterator& it) const { return this->nodes.top() == it.nodes.top(); }
			bool operator!=(const iterator& it) const { return this->nodes.top() != it.nodes.top(); }
			MCTS_Node& operator*();

			bool isExpanded() const { return nodes.top()->isExpanded(); }
			bool hasParent() const { return nodes.top()->m_parentPtr != nullptr; }
			bool hasChildren() const { return nodes.top()->m_childrenPtrs.size(); }
			bool isRoot() const { return nodes.top()->isRoot(); }
			bool isLeaf() const { return nodes.top()->isLeaf(); }

			void expand() { nodes.top()->expand(); }
			void prune() { nodes.top()->prune(); }

			// Moves iterator to parent node
			void toParent()
			{
				nodes.pop();	// empty stack means we are past the root
			}

			// Selects the child with the highest UCB value
			// Then moves to that child.
			// Warning: Before calling this method, make sure that the current node
			// has been expanded and also has children.
			// An error will occur, if this method is called on a node without
			// any children.
			// maximize - determines whether the selection should favor children which higher
			//				or lower UCB scores. if maximize == true, then method favors higher UCB scores
			void toBestUCB(bool maximize);

			// Selects child with the best average value.
			// Then moves to that child.
			// maximize = true:  look for max ucb. favors white player
			// maximize = false: look for min ucb. favors black player
			// * See comments for goToSelectedChild()
			void toBestAverage(bool maximize);

			void toMostVisited();

			// * See comments of goToSelectedChild()
			void toFirstChild();

		private:
			// node which iterator is currently referencing
			std::stack<MCTS_Node*> nodes;
		}; // end class iterator

		iterator root() { return iterator(this); }
	}; // end class MCTS_Node
} // namespace forge