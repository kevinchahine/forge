#include "tree.h"

#include "forge/search/MCTS_Node.h"

namespace forge
{
	namespace test
	{
		namespace tree
		{
			void print() {
				MCTS_Node root;

				root.expand();

				for (auto & child : root.children()) {
					child->expand();

					for (auto & grandchild : child->children()) {
						grandchild->expand();
					}
				}

				root.printTree(10);
			}
		} // namespace tree
	} // namespace test
} // namespace forge