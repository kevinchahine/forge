#include "UciMatch.h"

#include <uci/commands/position.h>

#include <thread>	// for hardware_concurrency()

using namespace std;

namespace forge
{
	namespace game
	{
		void UciMatch::play()
		{
			Position currPos;

			// --- Define Callbacks ---
			function<void(const uci::commands::setoption&)> on_setoption = [&](const uci::commands::setoption& cmd)
			{
				//cout << "We need to setoption: " << cmd << endl;
			};

			function<void(const uci::commands::position&)> on_position = [&](const uci::commands::position& cmd)
			{
				currPos = cmd.pos;
			};

			function<void(const uci::commands::ucinewgame&)> on_ucinewgame = [&](const uci::commands::ucinewgame& cmd) 
			{ 
				currPos.setupNewGame(); 
			};

			function<void(const uci::commands::go&)> on_go = [&](const uci::commands::go& cmd)
			{
				//uci::go g{ cmd };

				// TODO: A lot goes here
			};

			////m_uciGui.on_setoption = on_setoption;
			////m_uciGui.on_ucinewgame = on_ucinewgame;
			////m_uciGui.on_position = on_position;

			// --- Start Communicating ---

			m_uciGui.recv_until_uci();

			{
				uci::commands::id id;
				id.name = "Forge";
				m_uciGui.send_id(id);

				id.name.clear();
				id.author = "Kevin \'Micah\' Chahine";
				m_uciGui.send_id(id);
			}

			// .......... Send options here ............
			
			{
				uci::commands::option op = uci::commands::option::spin{};

				uci::commands::option::spin& spin = op.as<uci::commands::option::spin>();
				op.option_name = "MultiPV";
				spin.default_val = std::thread::hardware_concurrency();
				spin.min = 1;
				spin.max = std::thread::hardware_concurrency();
				spin.var = std::thread::hardware_concurrency();
				
				cout << "Sending op" << endl;
				m_uciGui.send_option(op);
				cout << "done" << endl;
			}

			{
				uci::commands::option op = uci::commands::option::string{};

				////op.option_name = "UCI_EngineAbout";
				////uci::commands::option::string& string_op = op.as<uci::commands::option::string&>(op);
				////string_op.default = "Hi. I am Forge, an AI powered chess engine written in C++ by Kevin Chahine";
				////
				////m_uciGui.send(op);
			}

			cout << "waiting for isready...";
			m_uciGui.recv_until_isready();	// setoption commands will be handled as we wait
			cout << "done" << endl;

			m_uciGui.send_readyok();
			
			cout << "waiting for position...";
			uci::commands::position pos = m_uciGui.recv_until_position();	// maybe we should wait for go instead or even stop?
			cout << "done" << endl;

			cout << "waiting for go...";
			uci::commands::go go = m_uciGui.recv_until_go();				// 
			cout << "done" << endl;

			cout << "waiting for stop...";
			m_uciGui.recv_until_stop();
			cout << "got it" << endl;

			cout << "waiting for quit...";
			m_uciGui.recv_until_quit();
			cout << "got it" << endl;
		}
	} // namespace game
} // namespace forge