#pragma once

#include "forge/controllers/MCTS_Base.h"

#include "forge/util/composite.h"

namespace forge
{
	class MCTS_Sequential : public MCTS_Base<forge::composite>
	{
	protected:// ------------------------- NESTED CLASS --------------------------
		class EvalVisitsPair
		{
		public:
			heuristic_t eval = 0;
			int visits = 0;
		};

	public:
		virtual std::string getNameVariant() const override { return "Sequential"; }

	protected:
		MCTS_Node::iterator select(MCTS_Node::iterator curr);

		EvalVisitsPair expandAndEvaluate(MCTS_Node::iterator curr);

		void backPropagate(MCTS_Node::iterator curr, const EvalVisitsPair & evalVisits);

	public:
		virtual void solve() override;
	};
} // namespace forge