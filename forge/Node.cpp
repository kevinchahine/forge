#include "Node.h"

#include "MoveGenerator.h"

using namespace std;

namespace forge
{
	void Node::reset()
	{
		if (m_lock.try_lock()) {
			m_lock.unlock();
		}
		else {
			cout << "Error: " << __FUNCTION__ << " m_lock was locked when it wan't supposed to be\n";
			return;
		}

		m_position.clear();
		m_nextPtr = nullptr;
		m_childrenPtrs.clear();
		m_state = STATE::FRESH;
	}

	void Node::expand()
	{
		m_lock.lock();

		MoveList moves;

		// 1.) --- Generate legal moves ---
		// Figures out wether white or black is playing and generates moves for them.
		moves = MoveGenerator::generatePseudoMoves(m_position);	

		// 2.) --- Create children nodes ---
		m_childrenPtrs.clear();
		m_childrenPtrs.reserve(moves.size());

		for (const auto & move : moves) {
			unique_ptr<Node> childPtr = make_unique<Node>();

			childPtr->m_move = move.move;
			childPtr->m_position = move.position;	// TODO: Slow copy

			m_childrenPtrs.push_back(std::move(childPtr));
		}

		// 3.) --- Assign sibling pointers ---
		for (size_t i = 0; i < m_childrenPtrs.size() - 1; i++) {
			unique_ptr<Node> & childPtr = m_childrenPtrs.at(i);
			unique_ptr<Node> & nextChildPtr = m_childrenPtrs.at(i + 1);

			childPtr->m_nextPtr = nextChildPtr.get();
		}

		// 4.) --- Assign last childs m_nextPtr to parent ---
		if (m_childrenPtrs.empty() == false) {
			m_childrenPtrs.back()->m_nextPtr = this;
		}

		m_state = STATE::EXPANDED;

		m_lock.unlock();
	}

	void Node::prune()
	{
		m_lock.lock();

		m_childrenPtrs.clear();

		m_state = STATE::PRUNED;

		m_lock.unlock();
	}

} // namespace forge