#pragma once

#include <forge/core/MoveGenerator2.h>
#include <forge/core/MovePositionPair.h>

#include <iostream>

namespace forge
{
	class ControllerBase
	{
	public:
		virtual void reset();

		virtual MovePositionPair getMove(const Position & position) = 0;

		virtual std::string getName() const = 0;

		virtual std::string getNameVariant() const = 0;

	protected:

	};
} // namespace forge