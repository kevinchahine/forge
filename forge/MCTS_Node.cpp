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

	float MCTS_Node::ucb() const
	{
		// TODO: Optimize: This is a potentially good thing to optimize
		// - Can log() be computed more efficiently?
		// - How about sqrt()?
		// - Can we use something other than float so that we don't have to share the FPU
		//	during multi-threading.
		float avg = this->t / this->n;
		float parent_visits = (this->m_parentPtr != nullptr ? this->m_parentPtr->n : 0.0f);
		float log_N = std::log(parent_visits);
		float ucb = avg + this->temperature * std::sqrt(log_N / this->n);

		return ucb;
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