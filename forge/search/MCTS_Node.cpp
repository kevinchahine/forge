#include "forge/source/globals.h"
#include "forge/search/MCTS_Node.h"

#include <guten/termcolor/termcolor.hpp>

#include <algorithm>
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
	
	bool compUCB(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right)
	{
		return left->ucb() < right->ucb();
	}
	
	// ----------------------------------- METHODS -------------------------------
	
	float MCTS_Node::ucb() const
	{
		return calcUCB(
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
		return *p_node;
	}

	void MCTS_Node::iterator::toBestUCB(bool maximize)
	{
		// Determine which child has the highest/lowest UCB score
		
		// Stores all the ucb scores of each child in the same order
		std::vector<std::shared_ptr<MCTS_Node>> & children = p_node->children();

		vector<float> ucbScores(children.size());

		// Copy the ucb scores of each child into ucbScores
		transform(
			children.begin(),
			children.end(),
			ucbScores.begin(),
			[&](const shared_ptr<MCTS_Node>& childPtr) { auto ucb = childPtr->ucb(); return (maximize ? ucb : -ucb); }
		);

		// Create Stochastic Universal Sampling distribution based on our scores
		discrete_distribution<size_t> dist(ucbScores.begin(), ucbScores.end());	// O(n)

		// Randomly select a child giving higher weight to samples with higher ucb scores
		std::vector<std::shared_ptr<MCTS_Node>>::iterator it = children.begin() + dist(g_rand);

		p_node = it->get();
	}
	
	void MCTS_Node::iterator::toBestAverage(bool maximize)
	{
		vector<shared_ptr<MCTS_Node>> & children = p_node->children();

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

		p_node = it->get();
	}

	void MCTS_Node::iterator::toFirstChild()
	{
		p_node = p_node->children().front().get();
	}
} // namespace forge
