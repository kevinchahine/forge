#pragma once

#include "Position.h"

#include <vector>
#include <memory>

namespace forge
{
	class Node
	{
	public:


	private:
		// Stores a position of the game
		Position position;

		// Address of parent node
		// if nullptr then this object is the root of the tree
		// No need to dereference
		Node * parentPtr = nullptr;
		
		// Address of next child node
		// move efficient than using an tree iterator
		// No need to dereference
		Node * siblingPtr = nullptr;

		// Addresses of children nodes
		std::vector<std::unique_ptr<Node>> childrenPtrs;
	};
} // namespace forge