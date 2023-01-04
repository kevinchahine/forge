#include "forge/source/globals.h"
#include "forge/search/MCTS_Node.h"
#include "forge/search/UCB.h"

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
		const shared_ptr<MCTS_Node> & left,
		const shared_ptr<MCTS_Node> & right) {
		return left->average() < right->average();
	}

	bool compUCB(
		const shared_ptr<MCTS_Node> & left,
		const shared_ptr<MCTS_Node> & right) {
		return left->ucb() < right->ucb();
	}

	bool compVisits(
		const shared_ptr<MCTS_Node> & left,
		const shared_ptr<MCTS_Node> & right) {
		return left->nVisits() < right->nVisits();
	}

	// ----------------------------------- METHODS -------------------------------

	void MCTS_Node::lastVisit() {
		mark = UCB::MARK_LAST_VISIT;
	}

	void MCTS_Node::expand() {
		// --- Generate Children ---
		// call expand() from super class
		this->super_t::expand();

		auto & children = this->children();

		const int nChildren = (int) children.size();

		for (int i = 0; i < nChildren - 1; ++i) {
			shared_ptr<MCTS_Node> & childPtr = children.at(i);
			shared_ptr<MCTS_Node> & nextPtr = children.at(i + 1);

			// --- Set Address of Siblings ---
			childPtr->m_nextPtr = nextPtr.get();

			// --- Default UCB Score ---
			// All children will initially have the same UCB score.
			childPtr->ucbScore = UCB::INIT;
		}
	}

	void MCTS_Node::update(float score, float nEvals) {
		// --- Update UCB values ---
		this->t += score;
		this->n += nEvals;

		ucbScore = UCB::calcUCB(
			this->t,
			this->parentPtr()->n,
			this->n,
			this->mark,
			temperature
		);
	}

	void MCTS_Node::merge(const MCTS_Node & node) {
		this->t += node.t;
		this->n += node.n;
		this->mark += node.mark;

		ucbScore = UCB::calcUCB(
			this->t,
			this->parentPtr()->n,
			this->n,
			this->mark,
			temperature
		);
	}

	void MCTS_Node::sort() {
		vector<shared_ptr<MCTS_Node>> & c = children();

		const int nChildren = (int) c.size();

		if (nChildren > 0) {
			// --- Insertion Sort (Max to Min) ---
			for (int i = 1; i < nChildren; ++i) {
				shared_ptr<MCTS_Node> key = c.at(i);
				int j = i - 1;

				while (j >= 0 && c.at(j)->ucb() < key->ucb()) {
					c.at(j + 1) = c.at(j);
					--j;
				}

				c.at(j + 1) = key;
			}

			// --- Reset Sibling Ptrs ---
			for (int i = 0; i < nChildren - 1; ++i) {
				c.at(i)->m_nextPtr = c.at(i + 1)->m_nextPtr;
			}

			c.back()->m_nextPtr = nullptr;
		}
	}

	// ----------------------------------- ITERATOR ------------------------------

	void MCTS_Node::iterator::toBestUCB(int nThBest) {
		// Determine which child has the highest/lowest UCB score

		// Stores all the ucb scores of each child in the same order
		std::vector<std::shared_ptr<MCTS_Node>> & children = p_node->children();

		p_node = children.at(nThBest).get();
	}

	void MCTS_Node::iterator::toBestAverage() {
		vector<shared_ptr<MCTS_Node>> & children = p_node->children();

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
		vector<shared_ptr<MCTS_Node>> & children = p_node->children();

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