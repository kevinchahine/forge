#pragma once

#include "BoardSquare.h"

namespace forge
{
	// A class that represents a Pin by storing the coordinates of 
	// both the pinned piece and the pinning (or pinner) piece.
	// Object is initialized with as a valid 
	class Pin
	{
	public:

		// Identifies if this represents a valid pin.
		// Meaning:
		//	- a pinned piece exists and
		//  - a pinner piece exists
		bool isValid() const {
			return pinned.isValid() && pinner.isValid() && (pinned != pinner);
		}

	public:
		BoardSquare pinned;
		BoardSquare pinner;
	};
} // namespace forge