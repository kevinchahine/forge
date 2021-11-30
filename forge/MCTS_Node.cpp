#include "MCTS_Node.h"

namespace forge
{
	float MCTS_Node::uct(float average, float temperature, int parentVisits, int currVisits)
	{
		// UCB = x_i + C * sqrt(ln(N) / n_i)
		return average + temperature * sqrt(log(parentVisits) / currVisits);
	}

	void MCTS_Node::reset()
	{
	}

	void MCTS_Node::expand()
	{
	}

	MCTS_Node::iterator& MCTS_Node::iterator::operator++()
	{
		return *this;
	}

	MCTS_Node& MCTS_Node::iterator::operator*()
	{
		return *p_node;
	}
} // namespace forge