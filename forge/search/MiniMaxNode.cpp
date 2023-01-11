#include "forge/search/MiniMaxNode.h"

#include <forge/core/MoveList.h>
#include <forge/core/MoveGenerator2.h>

#include <algorithm>

using namespace std;

namespace forge
{
	// Used for std::min_element() and std::max_element. 
	// Compares two shared_ptr<MiniMaxNode> based on fitness score of each MiniMaxNode.
	bool compareFitness(const shared_ptr<MiniMaxNode>& a, const shared_ptr<MiniMaxNode>& b) {
		return a->fitness() < b->fitness();
	}

	void MiniMaxNode::expand()
	{
		super_t::expand();

		// Used to prevent underflow (hint: unsigned 0 - 1 gives yeilds underflow)
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

/////	void MiniMaxNode::prune()
/////	{
/////		const auto& c = m_childrenPtrs;
/////
/////		vector<shared_ptr<MiniMaxNode>>::const_iterator it;
/////
/////		// Only makes sense for minimax
/////		// Find child with max or min fitness and assign it to this node.
/////		// Using min or max will depend on which player is moving.
/////		if (m_position.isWhitesTurn()) {
/////			// --- White player is moving ---
/////
/////			// White is maximizing so find child with highest fitness.
/////			// !!! If children vector is empty, then max_element will return iterator to end.
/////			// TODO: Optimize: Try using pointers for range instead of iterators
/////			it = max_element(
/////				c.begin(),		// c.data(), 
/////				c.end(),		// c.data() + c.size(),
/////				compareFitness);
/////		}
/////		else {
/////			// --- Black player is moving ---
/////
/////			// Black is minimizing so find child with lowest fitness
/////			// *See comments for max_element call above.
/////			// TODO: Optimize: Try using pointers for range instead of iterators
/////			it = min_element(
/////				c.begin(),			// c.data(),
/////				c.end(),			// c.data() + c.size(),
/////				compareFitness);
/////		}
/////
/////		// Assign pointer to best child if it exists.
/////
/////		if (it != c.end()) {
/////			p_bestChild = (it != c.end() ? *it : nullptr);	// m_bestChildPtr = p_bestChild;
/////			m_fitness = p_bestChild->fitness();
/////		}
/////		else {
/////#ifdef _DEBUG
/////			cout << termcolor::push
/////				<< termcolor::on_bright_red
/////				<< "Error: " << __FUNCTION__ << " line " << __LINE__
/////				<< ": No best child was found. Do any children exist. " << '\n'
/////				<< termcolor::pop;
/////#endif // _DEBUG
/////		}
/////
/////		// --- Call parents prune method ---
/////		// This will actually do the work of deleting child branches.
/////		super_t::prune();
/////	}

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
				//cout << termcolor::blue << "FRESH" << endl;
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
			//case STATE::PRUNED:
			//	//cout << "PRUNED\n";
			//	// We need to either go to next sibling or parent
			//	if (nextSiblingExists()) {
			//		goToNextSibling();
			//	}
			//	else {
			//		goToParent();
			//	}
			//
			//	break;
			default:	throw std::runtime_error("New State");			break;
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
		///case STATE::PRUNED:								break;
		default:	throw std::runtime_error("New State");	break;
		}

		return *p_node;
	}
} // namespace forge