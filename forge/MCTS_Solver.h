#pragma once
#include "SolverBase.h"
#include "MCTS_Node.h"

namespace forge
{
    class MCTS_Solver : public SolverBase
    {
    public:
		virtual void reset() override;

		virtual MovePositionPair getMove(const Position& position) override;

		virtual std::string getName() const override { return "MCTS"; }

		virtual std::string getNameVariant() const override { return "Basic"; }

		const MCTS_Node& nodeTree() const { return m_nodeTree; }

	protected:
		void traverse();

		void expand();

		int rollout();

		void backpropagate(int rolloutResult);
		
		// TODO: Could make this constant
		MovePositionPair selectBestMove();

	public:
		MovePositionPair solve(const Position& position);

    protected:
        MCTS_Node m_nodeTree;

		MCTS_Node::iterator it;
    };
} // namespace forge