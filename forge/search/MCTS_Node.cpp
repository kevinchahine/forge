#include "forge/source/globals.h"
#include "forge/search/MCTS_Node.h"

#include <guten/termcolor/termcolor.hpp>

#include <algorithm>
#include <functional>	// for std::plus
#include <random>

using namespace std;

namespace forge
{
	// ----------------------------------- STATIC --------------------------------

	const float MCTS_Node::temperature = 1.5f;

	bool compAverage(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right) {
		return left->average() < right->average();
	}

	bool compUCB(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right) {
		return left->ucb() < right->ucb();
	}

	bool compVisits(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right) {
		return left->nVisits() < right->nVisits();
	}

	// ----------------------------------- METHODS -------------------------------

	void MCTS_Node::lastVisit() {
		mark = -std::numeric_limits<float>::infinity();
		ucbScore = -std::numeric_limits<float>::infinity();
	}

	void MCTS_Node::expand() {
		// --- Generate Children ---
		// call expand() from super class
		this->super_t::expand();

		// --- Store in a Heap (priority queue) ---
		auto& children = this->children();

		if (children.size()) {
			// All children will initially have the same UCB score.
			const float UCB = MCTS_Node::calcUCB(
				children.front()->average(),
				children.front()->parentPtr()->n,
				children.front()->n,
				mark);

			for (size_t i = 0; i < children.size() - 1; ++i) {
				shared_ptr<MCTS_Node>& childPtr = children.at(i);
				shared_ptr<MCTS_Node>& nextPtr = children.at(i + 1);

				// --- Set Address of Siblings ---
				childPtr->m_nextPtr = nextPtr.get();

				// --- Default UCB Score ---
				childPtr->ucbScore = UCB;
			}
		}
	}

	void MCTS_Node::update(int score) {
		// --- Update UCB values ---
		t += score;
		n += 1.0f;
		ucbScore = calcUCB(
			this->average(),
			this->parentPtr()->n,
			this->n,
			this->mark);
	}

	void MCTS_Node::merge(const MCTS_Node& node) {
		this->t += node.t;
		this->n += node.n;
		this->mark += node.mark;

		this->ucbScore = calcUCB(
			this->average(),
			this->parentPtr()->n,
			this->n,
			this->mark);
	}

	void MCTS_Node::sort() {
		vector<shared_ptr<MCTS_Node>>& c = children();

		if (c.size()) {
			// --- Insertion Sort (Max to Min) ---
			for (int i = 1; i < c.size(); ++i) {
				shared_ptr<MCTS_Node> key = c.at(i);
				int j = i - 1;

				while (j >= 0 && c.at(j)->ucb() < key->ucb()) {
					c.at(j + 1) = c.at(j);
					--j;
				}

				c.at(j + 1) = key;
			}

			// --- Reset Sibling Ptrs ---
			for (size_t i = 0; i < c.size() - 1; ++i) {
				c.at(i)->m_nextPtr = c.at(i + 1)->m_nextPtr;
			}

			c.back()->m_nextPtr = nullptr;
		}
	}

	// ----------------------------------- ITERATOR ------------------------------

	MCTS_Node& MCTS_Node::iterator::operator*() {
		return *p_node;
	}

	void MCTS_Node::iterator::toBestUCB(int nThBest) {
		// Determine which child has the highest/lowest UCB score

		// Stores all the ucb scores of each child in the same order
		std::vector<std::shared_ptr<MCTS_Node>>& children = p_node->children();

		p_node = children.at(nThBest).get();

		//vector<float> ucbScores(children.size());
		//
		//bool isCompletelySearch = true;
		//
		//// Copy the ucb scores of each child into ucbScores
		//for (size_t n = 0; n < children.size(); n++) {
		//	const std::shared_ptr<MCTS_Node>& childPtr = children.at(n);
		//
		//	// Check to see if the node's subtree has already been fully searched.
		//	if (childPtr->isPruned()) {
		//		// Since this node has been fully searched. Give it a value which will not be selected for the search.
		//		ucbScores.at(n) = std::numeric_limits<float>::lowest();
		//	}
		//	else {
		//		// Recalculate ucb score
		//		isCompletelySearch = false;
		//		ucbScores.at(n) = childPtr->ucb();
		//	}
		//}
		//
		//// If all children are pruned (fully searched) then this node can be marked as pruned (fully searched).
		//if (isCompletelySearch == true) {
		//	cout << guten::color::lightred << "Node pruned" << guten::color::white << endl;
		//	nodes.top()->prune();
		//}
		//
		//std::vector<float>::const_iterator maxUcbIt =
		//	std::max_element(ucbScores.begin(), ucbScores.end());
		//
		//size_t maxUcbIndex = maxUcbIt - ucbScores.begin();
		//
		//// Randomly select a child giving higher weight to samples with higher ucb scores
		////std::vector<std::shared_ptr<MCTS_Node>>::const_iterator it = children.begin() + dist(g_rand);
		//std::vector<std::shared_ptr<MCTS_Node>>::const_iterator it = children.begin() + maxUcbIndex;
		//
		//nodes.push((*it).get());
	}

	void MCTS_Node::iterator::toBestAverage() {
		vector<shared_ptr<MCTS_Node>>& children = p_node->children();

		#ifdef _DEBUG
		if (children.empty()) {
			throw std::runtime_error("Error: No children exist.");
		}
		#endif

		vector<shared_ptr<MCTS_Node>>::iterator it;

		it = std::max_element(children.begin(), children.end(), compAverage);

		p_node = it->get();
	}

	void MCTS_Node::iterator::toMostVisited() {
		vector<shared_ptr<MCTS_Node>>& children = p_node->children();

		vector<shared_ptr<MCTS_Node>>::iterator it;

		it = std::max_element(children.begin(), children.end(), compVisits);

		p_node = it->get();
	}

	void MCTS_Node::iterator::toFirstChild() {
		p_node = p_node->children().back().get();
	}

	void MCTS_Node::iterator::toNext() {
		while (true) {
			if (p_node->hasNext()) {
				// --- Go to Sibling ---
				p_node = p_node->nextPtr();
				break;
			}

			if (p_node->hasParent()) {
				// --- Going to Aunt/Uncle (through parent) ---
				p_node = p_node->parentPtr();
			}
			else {
				// *** We reached the Root ***
				break;
			}
		}
	}
} // namespace forge