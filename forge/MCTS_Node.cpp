#include "MCTS_Node.h"

namespace forge
{
	float MCTS_Node::calcUCB(float average, float temperature, int parentVisits, int currVisits)
	{
		// UCB = x_i + C * sqrt(ln(N) / n_i)
		return average + temperature * sqrt(log(parentVisits) / currVisits);
	}

	void MCTS_Node::update(int rolloutResult)
	{
#ifdef _DEBUG
		if (this->isRoot()) {
			cout << guten::color::push() << guten::color::red
				<< "MCTS_Node::update() was called on root node."
				<< endl << guten::color::pop();
		}
#endif 
		t += rolloutResult;
		n += 1.0f;
		int parentVisits = parentPtr()->nGamesVisited();
		m_ucb = calcUCB(t / n, temperature, parentVisits, n);

		//cout
		//	<< "t         " << t << endl
		//	<< "n         " << n << endl
		//	<< "N         " << parentVisits << endl
		//	<< "ucb       " << m_ucb << endl
		//	<< "Press any key" << endl;
		//cin.get();
	}

	void MCTS_Node::updateRoot(int rolloutResult)
	{
		t += rolloutResult;
		n += 1.0f;
	}

	MCTS_Node::iterator& MCTS_Node::iterator::operator++()
	{
		return *this;
	}

	MCTS_Node& MCTS_Node::iterator::operator*()
	{
		return *p_node;
	}

	bool compUCB(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right)
	{
		///cout << __FUNCTION__ << '\t' << left->ucb() << " < " << right->ucb() << endl;

		return left->ucb() < right->ucb();
	}

	bool compAverage(
		const shared_ptr<MCTS_Node>& left,
		const shared_ptr<MCTS_Node>& right)
	{
		///cout << __FUNCTION__ << '\t' << left->average() << " < " << right->average() << endl;

		return left->average() < right->average();
	}

	void MCTS_Node::iterator::goToSelectedChild()
	{
#ifdef _DEBUG
		cout << guten::color::push();

		if (p_node == nullptr) {
			cout << guten::color::red << "ERROR: p_node is null" << endl;
		}
		else if (p_node->children().empty()) {
			cout << guten::color::red << "ERROR: node does not have children" << endl;
		}

		cout << guten::color::pop();
#endif // _DEBUG

		auto& c = p_node->children();

		vector<shared_ptr<MCTS_Node>>::iterator it = max_element(c.begin(), c.end(), compUCB);

		p_node = it->get();
	}

	void MCTS_Node::iterator::goToBestChild()
	{
		auto& c = p_node->children();

		vector<shared_ptr<MCTS_Node>>::iterator it = max_element(c.begin(), c.end(), compAverage);

		if (it == c.end()) {
			cout << "Error: Best child was not found among the " << c.size() << endl;
		}

		p_node = it->get();
	}

	void MCTS_Node::iterator::goToFirstChild()
	{
#ifdef _DEBUG
		cout << guten::color::push();

		if (p_node == nullptr) {
			cout << guten::color::red << "ERROR: p_node is null" << endl;
		}
		else if (p_node->children().empty()) {
			cout << guten::color::red << "ERROR: node does not have children" << endl;
		}

		cout << guten::color::pop();
#endif // _DEBUG

		auto& c = p_node->children();

		p_node = c.front().get();
	}
} // namespace forge