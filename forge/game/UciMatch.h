#pragma once

#include <forge/controllers/ControllerBase.h>

#include <ucilib/uci/gui.h>

namespace forge
{
	namespace game
	{
		class UciMatch
		{
		public:
			void play();

		private:
			uci::gui m_uciGui;

			std::unique_ptr<ControllerBase> m_whitesController;
		};
	} // namespace game
} // namespace forge