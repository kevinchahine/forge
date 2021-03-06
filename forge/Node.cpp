#include "Node.h"

using namespace std;

namespace forge
{
	void Node::expand()
	{
		MoveList moves;

		// 1.) --- Generate legal moves ---
		if (m_position.moveCounter().isWhitesTurn()) {
			moves = m_position.board().generatePsuedoLegalMovesWhite();	// TODO: these should generate positions
		}
		else {
			moves = m_position.board().generatePsuedoLegalMovesBlack();	// TODO: these should generate positions
		}

		// 2.) --- Create children nodes ---
		m_childrenPtrs.clear();
		m_childrenPtrs.reserve(moves.size());

		for (const auto & move : moves) {
			unique_ptr<Node> childPtr = make_unique<Node>();

			//m_childrenPtrs.back()->m_position.applyMove(move.second);	// TODO:
			childPtr->m_parentPtr = this;

			m_childrenPtrs.push_back(std::move(childPtr));
		}

		// 3.) --- Fix sibling pointers ---
		for (size_t i = 0; i < m_childrenPtrs.size() - 1; i++) {
			unique_ptr<Node> & childPtr = m_childrenPtrs.at(i);
			unique_ptr<Node> & nextChildPtr = m_childrenPtrs.at(i + 1);

			childPtr->m_siblingPtr = nextChildPtr.get();
		}
	}
} // namespace forge