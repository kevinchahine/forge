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
		const shared_ptr<MCTS_Node>& right)
	{
		return left->average() < right->average();
	}

	//bool compUCB(
	//	const shared_ptr<MCTS_Node>& left,
	//	const shared_ptr<MCTS_Node>& right)
	//{
	//	return left->ucb() < right->ucb();
	//}

	bool compVisits(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right)
	{
		return left->nVisits() < right->nVisits();
	}

	// ----------------------------------- METHODS -------------------------------

	//float MCTS_Node::ucb() const
	//{
	//	return calcUCB(
	//		this->average(),
	//		this->parentPtr()->n,
	//		this->n);
	//}

	float MCTS_Node::ucbWhite() const
	{
		return calcUCBWhite(
			this->average(),
			this->parentPtr()->n,
			this->n);
	}

	float MCTS_Node::ucbBlack() const
	{
		return calcUCBBlack(
			this->average(),
			this->parentPtr()->n,
			this->n);
	}

	void MCTS_Node::update(int score)
	{
		t += score;
		n += 1.0f;
	}

	void MCTS_Node::merge(const MCTS_Node& node)
	{
		this->t += node.t;
		this->n += node.n;
	}

	// ----------------------------------- ITERATOR ------------------------------

	MCTS_Node& MCTS_Node::iterator::operator*()
	{
		return *nodes.top();
	}

	void MCTS_Node::iterator::toBestUCB(bool maximize)
	{
		// Determine which child has the highest/lowest UCB score

		// Stores all the ucb scores of each child in the same order
		std::vector<std::shared_ptr<MCTS_Node>>& children = nodes.top()->children();

		vector<float> ucbScores(children.size());

		bool isCompletelySearch = true;

		// Copy the ucb scores of each child into ucbScores
		for (size_t n = 0; n < children.size(); n++) {
			const std::shared_ptr<MCTS_Node>& childPtr = children.at(n);

			// Check to see if the node's subtree has already been fully searched.
			if (childPtr->isPruned()) {
				// Since this node has been fully searched. Give it a value which will not be selected for the search.
				ucbScores.at(n) = (maximize ? std::numeric_limits<float>::lowest() : std::numeric_limits<float>::max());
			}
			else {
				// Recalculate ucb score
				isCompletelySearch = false;
				ucbScores.at(n) = (maximize ? childPtr->ucbWhite() : childPtr->ucbBlack());
			}
		}

		// If all children are pruned (fully searched) then this node can be marked as pruned (fully searched).
		if (isCompletelySearch == true) {
			nodes.top()->prune();
		}
		
		std::vector<float>::const_iterator maxUcbIt;

		if (maximize)
			maxUcbIt = std::max_element(ucbScores.begin(), ucbScores.end());
		else
			maxUcbIt = std::min_element(ucbScores.begin(), ucbScores.end());

		size_t maxUcbIndex = maxUcbIt - ucbScores.begin();

		// TODO: Add weighted random selection to make engine less predictable.
		//// Make sure all numbers are greater than 0.0 (and sum is not equal to 0.0)
		//float min = *std::min_element(ucbScores.begin(), ucbScores.end());
		//transform(
		//	ucbScores.begin(), 
		//	ucbScores.end(), 
		//	ucbScores.begin(), 
		//	[min](float ucb) { return ucb - min + 0.0001; }
		//);
		//
		//// Create Stochastic Universal Sampling distribution based on our scores
		//discrete_distribution<size_t> dist(ucbScores.begin(), ucbScores.end());	// O(n)

		// Randomly select a child giving higher weight to samples with higher ucb scores
		//std::vector<std::shared_ptr<MCTS_Node>>::const_iterator it = children.begin() + dist(g_rand);
		std::vector<std::shared_ptr<MCTS_Node>>::const_iterator it = children.begin() + maxUcbIndex;

		nodes.push((*it).get());
	}

	void MCTS_Node::iterator::toBestAverage(bool maximize)
	{
		vector<shared_ptr<MCTS_Node>>& children = nodes.top()->children();

#ifdef _DEBUG
		if (children.empty()) {
			throw std::runtime_error("Error: No children exist.");
		}
#endif

		vector<shared_ptr<MCTS_Node>>::iterator it;

		if (maximize) {
			it = std::max_element(children.begin(), children.end(), compAverage);
		}
		else {
			it = std::min_element(children.begin(), children.end(), compAverage);
		}
		
		nodes.push(it->get());
	}
	
	void MCTS_Node::iterator::toMostVisited()
	{
		vector<shared_ptr<MCTS_Node>>& children = nodes.top()->children();
		
		vector<shared_ptr<MCTS_Node>>::iterator it;

		it = std::max_element(children.begin(), children.end(), compVisits);

		nodes.push(it->get());
	}
	
	void MCTS_Node::iterator::toFirstChild()
	{
		nodes.push(nodes.top()->children().front().get());
	}
} // namespace forge
