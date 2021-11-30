#include "MCTS_Node.h"

namespace forge
{
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