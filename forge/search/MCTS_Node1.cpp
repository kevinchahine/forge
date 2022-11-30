#include "forge/search/MCTS_Node1.h"
#include "forge/source/globals.h"

#include <guten/termcolor/termcolor.hpp>

#include <random>

using namespace std;

namespace forge
{
	// Selects a random child using Stochastic Universal Sampling (SUS)
	// favorHigh - controls whether higher number or lower number are given greater weights.
	//		should be true  if white player is selecting
	//		should be false if black player is selecting
	vector<shared_ptr<MCTS_Node1>>::iterator sampleRandomChild(vector<shared_ptr<MCTS_Node1>>& children, bool favorHigh)
	{
		// Stores all the ucb scores of each child in the same order
		vector<float> ucbScores(children.size());
		
		// Copy the ucb scores of each child into ucbScores
		transform(
			children.begin(),
			children.end(),
			ucbScores.begin(),
			[&](const shared_ptr<MCTS_Node1>& childPtr) { auto ucb = childPtr->ucb(); return (favorHigh ? ucb : -ucb); }
		);
		
		// Create Stochastic Universal Sampling distribution based on our scores
		discrete_distribution<size_t> dist(ucbScores.begin(), ucbScores.end());	// O(n)
		
		// Randomly select a child giving higher weight to samples with higher ucb scores
		return children.begin() + dist(g_rand);
	}

	bool compUCB(
		const shared_ptr<MCTS_Node1>& left,
		const shared_ptr<MCTS_Node1>& right)
	{
		///cout << __FUNCTION__ << '\t' << left->ucb() << " < " << right->ucb() << endl;

		return left->ucb() < right->ucb();
	}

	bool compAverage(
		const shared_ptr<MCTS_Node1>& left,
		const shared_ptr<MCTS_Node1>& right)
	{
		///cout << __FUNCTION__ << '\t' << left->average() << " < " << right->average() << endl;

		return left->average() < right->average();
	}

	void MCTS_Node1::iterator::goToSelectedChild(bool maximize)
	{
#ifdef _DEBUG
		cout << termcolor::push;

		if (p_node == nullptr) {
			cout << termcolor::red << "ERROR: p_node is null" << endl;
		}
		else if (p_node->children().empty()) {
			cout << termcolor::red << "ERROR: node does not have children" << endl;
		}

		cout << termcolor::pop;
#endif // _DEBUG

		auto& c = p_node->children();

		vector<shared_ptr<MCTS_Node1>>::iterator it;
		
		if (maximize) {
			//it = max_element(c.begin(), c.end(), compUCB);	// Maximize UCB
			it = sampleRandomChild(c, maximize);				// Stochastic Random Sampling (favor higher scores)
		}
		else {
			//it = min_element(c.begin(), c.end(), compUCB);	// Minimize UCB
			it = sampleRandomChild(c, maximize);				// Stochastic Random Sampling (favor lower scores)
		}
		
#ifdef _DEBUG
		if (it == c.end()) {
			cout << "Error: Best child was not found among the " << c.size() << endl;
		}
#endif // _DEBUG

		p_node = it->get();
	}

	void MCTS_Node1::iterator::goToBestChild()
	{
		auto& c = p_node->children();

		vector<shared_ptr<MCTS_Node1>>::iterator it = max_element(c.begin(), c.end(), compAverage);

		if (it == c.end()) {
			cout << "Error: Best child was not found among the " << c.size() << endl;
		}

		p_node = it->get();
	}

	void MCTS_Node1::iterator::goToFirstChild()
	{
#ifdef _DEBUG
		cout << termcolor::push;

		if (p_node == nullptr) {
			cout << termcolor::red << "ERROR: p_node is null" << endl;
		}
		else if (p_node->children().empty()) {
			cout << termcolor::red << "ERROR: node does not have children" << endl;
		}

		cout << termcolor::pop;
#endif // _DEBUG

		auto& c = p_node->children();

		p_node = c.front().get();
	}
} // namespace forge