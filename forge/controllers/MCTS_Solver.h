#pragma once

#include "forge/controllers/SolverBase.h"
#include "forge/search/MCTS_Node.h"

#include <set>

namespace forge
{
	class MCTS_Solver : public SolverBaseTemplate<forge::composite>
	{
	public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "Sequential"; }

		const MCTS_Node& nodeTree() const { return m_nodeTree; }

	protected:

		// TODO: Could make this constant
		MovePositionPair selectBestMove();

	public:
		MovePositionPair solve(const Position& position);

	protected:
		MCTS_Node m_nodeTree;

		MCTS_Node::iterator curr;
	};
} // namespace forge