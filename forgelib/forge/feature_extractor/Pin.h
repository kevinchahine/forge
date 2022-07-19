#pragma once

#include "forge/BoardSquare.h"
#include "forge/Direction.h"	

namespace forge
{
	// --- Forward Declarations ---
	class MoveGenerator2;

	// A class that represents a Pin by storing the coordinates of 
	// both the pinned piece and the pinning (or pinner) piece.
	// Object is initialized as an invalid Pin
	class Pin
	{
	public: // ------------------------ METHODS -------------------------------

		// Identifies if this represents a valid pin.
		// Meaning:
		//	- a pinned piece exists and
		//  - a pinner piece exists
		bool isValid() const {
			return pinned.isValid() && pinner.isValid() && (pinned != pinner);
		}

	public: // ------------------------ FIELDS --------------------------------
		BoardSquare pinned;
		BoardSquare pinner;
	};
} // namespace forge

#include "PinDefinitions.h"