#include "UciMatch.h"

#include <uci/commands/position.h>

using namespace std;

namespace forge
{
	namespace game
	{
		void UciMatch::play()
		{
			Position currPos;

			// --- Define Callbacks ---
			function<void(const uci::Command&)> on_setoption = [&](const uci::Command& cmd)
			{
				//cout << "We need to setoption: " << cmd << endl;
			};

			function<void(const uci::Command&)> on_position = [&](const uci::Command& cmd)
			{
				uci::commands::position pos = cmd;
				
				currPos = pos.to_position();
			};

			function<void(const uci::Command&)> on_ucinewgame = [&](const uci::Command& cmd) 
			{ 
				currPos.setupNewGame(); 
			};

			function<void(const uci::Command&)> on_go = [&](const uci::Command& cmd)
			{
				//uci::go g{ cmd };

			};

			m_uciGui.on_setoption = on_setoption;
			m_uciGui.on_ucinewgame = on_ucinewgame;
			m_uciGui.on_position = on_position;

			// --- Start Communicating ---

			m_uciGui.recv_until_uci();
			m_uciGui.send_id("Forge", "Kevin \'Micah\' Chahine");
			
			// .......... Send options here ............
			
			m_uciGui.send_uciok();
			
			m_uciGui.recv_until_isready();
			m_uciGui.send_readyok();


		}
	} // namespace game
} // namespace forge