#include "UciEngine.h"

#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>

using namespace std;
namespace fs = boost::filesystem;
namespace bp = boost::process;

namespace forge
{
	namespace uci
	{
		void UciEngine::launch()
		{
			// 1.) --- Search for the stockfish executable ---
			fs::path pathToEngine = 
				//bp::search_path();
				R"dil(C:\Program Files\stockfish_13_win_x64_bmi2\stockfish_13_win_x64_bmi2.exe)dil";
			
			launch(pathToEngine);
		}

		void UciEngine::launch(const std::string& engineName)
		{
			fs::path path = engineName;

			launch(path);
		}

		void UciEngine::launch(const boost::filesystem::path& path)
		{
			// 1.) --- Make sure file exists and is an executable ---
			if (fs::exists(path) == false) {
				throw std::exception("Could not open executable");
			}

			// 2.) --- Open as a child process ---
			m_child = bp::child(path, bp::std_in < pout, bp::std_out > pin);

			// 3.) --- Make sure chess engine is running ---
			if (m_child.running() == false) {
				throw std::exception("Could not run chess engine");
			}

			// 4.) --- Put engine in UCI mode ---
			uci();

			// *** Here engine will: ***
			//	- switch to UCI mode
			//	- identify itself with the id message
			//	- list options that it supports (if any)
			//	- finish by replying "uciok"
			// readId();
			// readOptions();
			// readUciok();

			// 5.) --- Send options is any ---
			// ... Nothing yet

			// 6.) --- Wait for "readyok" ---
			isready();
			// readReadyOk();
		}

		int UciEngine::eval(const Position& pos)
		{
			// 1.) --- Tell engine to expect a new game ---
			ucinewgame();

			// 2.) --- Set current position ---
			position(pos);

			// 3.) --- Start eval ---
			UciSearchCommands sCmd;
			sCmd.nodes = 100'000;
			go(sCmd);

			// *** Here engine will evaluate current position ***
			// And periodically print 'info' messages about its progress
			// When it finishes it will print 'bestmove <move> ...'
			// Warning: If engine was given the 'go infinite' or 'go' command
			//	we need to tell it to stop by issuing the 'stop' command

			// 4.) --- Wait till engine finishes ---
			//int eval = readBestMove();

			return 0;
		}
		
		MoveList UciEngine::movegen(const Position& pos)
		{
			MoveList moves;

			// 1.) --- Tell engine to expect a new game ---
			ucinewgame();

			// 2.) --- Set options (Optional) ---
			setOption("MultiPV", "500");

			// 2.) --- Set current position ---
			position(pos);

			// 3.) --- Start Move Generation ---
			UciSearchCommands sCmd;
			sCmd.depth = 1;
			go(sCmd);

			// *** Here engine will evaluate current position ***
			// And periodically print 'info' messages about its progress
			// When it finishes it will print 'bestmove <move> ...'
			// Warning: If engine was given the 'go infinite' or 'go' command
			//	we need to tell it to stop by issuing the 'stop' command

			// 4.) --- Wait till engine finishes ---
			// int eval = readEval();
			
			return moves;
		}

		void UciEngine::close()
		{
			quit();	// engine process should close on its own as soon as it can.

			// If engine doesn't close, the destructor will forcefully close it.
		}

		// ------------------------ UCI COMMANDS ------------------------------------
		
		void UciEngine::uci()
		{
			pout << "uci" << endl;
		}

		void UciEngine::debug(bool isDebug)
		{
			pout << "debug " << (isDebug ? "on" : "off") << endl;
		}

		void UciEngine::isready()
		{
			pout << "isready" << endl;
		}

		void UciEngine::setOption(const std::string& opName, const std::string& opVal)
		{
			pout << "setoption name " << opName << " value " << opVal << endl;
		}
		
		void UciEngine::ucinewgame()
		{
			pout << "ucinewgame" << endl;
		}

		void UciEngine::position()
		{
			pout << "position startpos" << endl;
		}

		void UciEngine::position(const std::string& fen)
		{
			pout << "position " << fen << endl;
		}

		void UciEngine::position(const Position& pos)
		{
			pout << "position " << pos.toFEN() << endl;
		}

		void UciEngine::go()
		{
			pout << "go" << endl;
		}

		void UciEngine::go(const UciSearchCommands& sCMD)
		{
			pout << "go " << sCMD << endl;
		}

		void UciEngine::stop()
		{
			pout << "stop" << endl;
		}

		void UciEngine::ponderhit()
		{
			pout << "ponderhit" << endl;
		}

		void UciEngine::quit()
		{
			pout << "quit" << endl;
		}

	} // namespace uci
} // namespace forge