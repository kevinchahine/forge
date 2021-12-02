#include "MiniMaxNode.h"

#include "MoveList.h"
#include "MoveGenerator2.h"

#include <algorithm>

using namespace std;

namespace forge
{
	// Used for std::min_element() and std::max_element. 
	// Compares two shared_ptr<MiniMaxNode> based on fitness score of each MiniMaxNode.
	bool compareFitness(const shared_ptr<MiniMaxNode>& a, const shared_ptr<MiniMaxNode>& b) {
		return a->fitness() < b->fitness();
	}

	void MiniMaxNode::reset()
	{
		super_t::reset();

		m_nextPtr = nullptr;
		m_fitness = 0;
		m_bestChildPtr = nullptr;
	}

	void MiniMaxNode::expand()
	{
		super_t::expand();

		// used to prevent underflow (hint: 0 - 1)
		if (m_childrenPtrs.empty() == false) {
			// 3.) --- Assign sibling pointers ---
			for (size_t i = 0; i < m_childrenPtrs.size() - 1; i++) {
				shared_ptr<MiniMaxNode>& childPtr = m_childrenPtrs.at(i);
				shared_ptr<MiniMaxNode>& nextChildPtr = m_childrenPtrs.at(i + 1);

				childPtr->m_nextPtr = nextChildPtr.get();
			}

			// 4.) --- Assign last childs m_nextPtr to parent ---
			m_childrenPtrs.back()->m_nextPtr = nullptr;
		}
	}

	MiniMaxNode::iterator& MiniMaxNode::iterator::operator++()
	{
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
			// --- Depth Limit has not been reached ---
			switch (p_node->m_state)
			{
			case STATE::FRESH:
				//cout << guten::color::blue << "FRESH" << endl;
				p_node->expand();

				// Were any children created?
				if (firstChildExists()) {
					// Yes. Atleast one child was created. Lets search it.
					goToFirstChild();
				}
				else {
					// No children where created.  Must be a checkmate.
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
				//cout << "EXPANDED\n";
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
				//cout << "PRUNED\n";
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

	MiniMaxNode& MiniMaxNode::iterator::operator*()
	{
		switch (p_node->m_state)
		{
		case STATE::FRESH:		/*p_node->expand();	*/	break;
		case STATE::EXPANDED:							break;
		case STATE::PRUNED:								break;
		default:	throw std::exception("New State");	break;
		}

		return *p_node;
	}
} // namespace forge