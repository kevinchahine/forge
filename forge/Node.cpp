#include "Node.h"

#include "MoveGenerator.h"

#include <algorithm>

using namespace std;

namespace forge
{
	// Used in std::min() and std::max. 
	// Compares two unique_ptr<Node> based on fitness score of each Node.
	bool compareFitness(const unique_ptr<Node> & a, const unique_ptr<Node> & b) {
		return a->fitness() < b->fitness();
	}

	void Node::reset()
	{
		m_position.clear();
		m_parentPtr = nullptr;
		m_nextPtr = nullptr;
		m_childrenPtrs.clear();
		m_fitness = 0;
		m_bestMove;
		m_state = STATE::FRESH;
	}

	void Node::expand()
	{
		///m_lock.lock();

		MoveList moves;

		// 1.) --- Generate legal moves ---
		// Figures out wether white or black is playing and generates moves for them.
		moves = MoveGenerator::generatePseudoMoves(m_position);

		///cout << moves.size() << " moves generated\n";

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

			childPtr->m_parentPtr = this;
			childPtr->m_nextPtr = nextChildPtr.get();
		}

		// 4.) --- Assign last childs m_nextPtr to parent ---
		if (m_childrenPtrs.empty() == false) {
			m_childrenPtrs.back()->m_parentPtr = this;
			m_childrenPtrs.back()->m_nextPtr = nullptr;
		}

		m_state = STATE::EXPANDED;

		///m_lock.unlock();
	}

	void Node::prune()
	{
		const auto & c = m_childrenPtrs;

		const Node * bestChild = nullptr;

		// Only makes sense for minimax
		// Find child with max or min fitness and assign it to this node.
		// Using min or max will depend on which player is moving.
		if (m_position.moveCounter().isWhitesTurn()) {
			// --- White player is moving ---

			// White is maximizing so find child with highest fitness
			bestChild = max_element(
				c.data(), 
				c.data() + c.size(),
				compareFitness)->get();		// goes from <unique_ptr *> to <*>
		}
		else {
			// --- Black player is moving ---

			// Black is minimizing so find child with lowest fitness
			bestChild = min_element(
				c.data(),
				c.data() + c.size(),
				compareFitness)->get();		// goes from <unique_ptr *> to <*>
		}

		m_fitness = bestChild->fitness();
		m_bestMove = bestChild->move();

		m_childrenPtrs.clear();

		m_state = STATE::PRUNED;
	}

	Node::iterator & Node::iterator::operator++()
	{
		///cout << depth << ' ' << depthLimit << '\n';
		// Did we reach depth limit?
		if (depth >= depthLimit) {
			///cout << "Limit reached\n";
			// We don't want to go down anymore.
			// Either we should go right or up
			// We need to either go to next sibling or parent
			if (nextSiblingExists()) {
				goToNextSibling();
			}
			else {
				goToParent();
			}
		}
		else {
			switch (ptr->m_state)
			{
			case STATE::FRESH:
				///cout << "FRESH" << endl;
				ptr->expand();

				// How many children where created?
				if (firstChildExists()) {
					goToFirstChild();
				}
				else {
					// We need to either go to next sibling or parent
					if (nextSiblingExists()) {
						goToNextSibling();
					}
					else {
						goToParent();
						// *** If we're pointing to nullptr then we have passed the root node.
					}
				}

				break;
			case STATE::EXPANDED:
				///cout << "EXPANDED\n";
				// We typically want to go to 1st child
				if (firstChildExists()) {
					goToFirstChild();
				}
				else {
					// Zero, No children. 

					// We need to either go to next sibling or parent
					if (nextSiblingExists()) {
						goToNextSibling();
					}
					else {
						goToParent();
						// *** If we're pointing to nullptr then we have passed the root node.
					}
				}

				break;
			case STATE::PRUNED:
				///cout << "PRUNED\n";
				// We need to either go to next sibling or parent
				if (nextSiblingExists()) {
					goToNextSibling();
				}
				else {
					goToParent();
				}

				break;
			default:	throw std::exception("New State");			break;
			}
		}

		return *this;
	}

	Node & Node::iterator::operator*()
	{
		switch (ptr->m_state)
		{
		case STATE::FRESH:		/*ptr->expand();	*/		break;
		case STATE::EXPANDED:							break;
		case STATE::PRUNED:								break;
		default:	throw std::exception("New State");	break;
		}

		return *ptr;
	}
	
} // namespace forge