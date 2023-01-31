#pragma once

#include "forge/controllers/MCTS_Base.h"

#include "forge/util/composite.h"

namespace forge
{
	class MCTS_Sequential : public MCTS_Base<forge::composite>
	{	
	public:
		virtual std::string getNameVariant() const override { return "Sequential"; }

	protected:
		MCTS_Node::iterator select(MCTS_Node::iterator curr);

		void expand(MCTS_Node::iterator curr);

		EvalVisits evaluate(MCTS_Node::iterator curr);

		void backPropagate(MCTS_Node::iterator curr, EvalVisits ev);

	public:
		virtual void solve() override;
	};
} // namespace forge