#include "Node.h"

#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	void Node::expand()
	{
		MoveList moves;

		// 1.) --- Generate legal moves ---
		// Figures out wether white or black is playing and generates moves for them.
		moves = MoveGenerator::generatePseudoMoves(m_position);	

		// 2.) --- Create children nodes ---
		m_childrenPtrs.clear();
		m_childrenPtrs.reserve(moves.size());

		for (const auto & move : moves) {
			unique_ptr<Node> childPtr = make_unique<Node>();

			//m_childrenPtrs.back()->m_position.applyMove(move.second);	// TODO:
			childPtr->m_parentPtr = this;

			m_childrenPtrs.push_back(std::move(childPtr));
		}

		// 3.) --- Assign sibling pointers ---
		for (size_t i = 0; i < m_childrenPtrs.size() - 1; i++) {
			unique_ptr<Node> & childPtr = m_childrenPtrs.at(i);
			unique_ptr<Node> & nextChildPtr = m_childrenPtrs.at(i + 1);

			childPtr->m_nextSiblingPtr = nextChildPtr.get();
		}

		// 4.) --- Set last childs sibling to nullptr ---
		if (m_childrenPtrs.empty() == false) {
			m_childrenPtrs.back()->m_nextSiblingPtr = nullptr;
		}
	}

} // namespace forge