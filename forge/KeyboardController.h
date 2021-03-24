#pragma once

#include "ControllerBase.h"

namespace forge
{
	class KeyboardController : public ControllerBase
	{
	public:
		virtual void reset() override;

		virtual Move getMove(const Position & position) override;

		virtual std::string getName() override;

	protected:

	};
} // namespace forge