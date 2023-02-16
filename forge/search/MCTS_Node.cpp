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

		for (int i = 0; i < nChildren; ++i) {
			shared_ptr<MCTS_Node> & childPtr = children.at(i);
			
			// --- Default UCB Score ---
			// All children will initially have the same (maxed out) UCB score.
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

	void MCTS_Node::updateRoot(float score, float nEvals) {
	// --- Update UCB values ---
		this->t += score;
		this->n += nEvals;

		// !!! Do not calculate the UCB Score !!!
		// Root nodes do not have UCB scores 
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
		}
	}

	vector<const Position *> MCTS_Node::getChildrenPositions() {
		const auto & children = (*this).children();

		vector<const Position *> pChildren;

		pChildren.reserve(children.size());

		for (const auto & child : children) {
			pChildren.emplace_back(&child->position());// get address of child position
		}

		return pChildren;
	}

	float MCTS_Node::updateChildrenUCB(const vector<heuristic_t> & childrenEvals) {
		const auto & children = this->children();

		#if _DEBUG
		assert(children.size() == childrenEvals.size());
		#endif 

		float sum = 0.0f;

		for (size_t i = 0; i < childrenEvals.size(); i++) {
			const auto & child = children.at(i);
			float eval = UCB::mapRange(childrenEvals.at(i));

			child->update(eval);

			sum += eval;
		}

		return sum;
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
	
	void MCTS_Node::iterator::toBestStochastic() {
		vector<shared_ptr<MCTS_Node>> & children = p_node->children();

		// --- Count number of visits ---
		vector<int> nVisits(children.size());

		// Sum of all visits
		int sum = 0;

		for (int i = 0; i < nVisits.size(); i++) {
			nVisits.at(i) = children.at(i)->nVisits();
			sum += nVisits.at(i);
		}

		int average = sum / nVisits.size();

		// --- Find all moves which are better than average ---
		vector<int> bestEvals;
		vector<int> bestIndex;

		for (int i = 0; i < nVisits.size(); i++) {
			if (nVisits.at(i) > average) {
				bestEvals.push_back(nVisits.at(i));
				bestIndex.push_back(i);
			}
		}

		// --- Stochastic Selection ---
		discrete_distribution<> dist(bestEvals.begin(), bestEvals.end());

		size_t nBestIndex = dist(g_rand);

		size_t selectionIndex = bestIndex.at(nBestIndex);

		p_node = children.at(selectionIndex).get();
	}
} // namespace forge