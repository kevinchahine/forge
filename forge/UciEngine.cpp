#include "UciEngine.h"

#include <boost/asio/read_until.hpp>
#include <boost/bind.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>

#include <string>
#include <functional>
#include <chrono>

using namespace std;
namespace fs = boost::filesystem;
namespace bp = boost::process;

namespace forge
{
	namespace uci
	{
		// ------------------------ HANDLER CHAINS ----------------------------------

		void on_recv_launch1(UciEngine& engine, const boost::system::error_code& ec, size_t size);
		void on_recv_launch2(UciEngine& engine, const boost::system::error_code& ec, size_t size);
		
		// TODO: Reimplement these callback functions as classes with functors.
		// Then make a single template function for async_read_until in such a way that we can 
		// do:
		//	async_read_until<OnRecvLaunch1>(engine);
		// --- instead of ---
		// boost::asio::async_read_until(
		//	engine.pin,
		// 		engine.buf_in,
		// 		'\n',
		// 		[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
		//	);

		void on_recv_launch1(UciEngine& engine, const boost::system::error_code& ec, size_t size)
		{
			// 1.) --- Get the line that we just read ---
			stringstream ss = engine.readLine();

			////cout << ss.str() << endl;

			// 2.) --- Extract the command token ---
			string cmd;
			ss >> cmd;

			// 3.) --- Determine how to handle the command ---

			if (cmd == "id") { 
				engine.handle_id(ss); 
			
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
				);
			}
			else if (cmd == "option") { 
				engine.handle_option(ss); 
				
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
				);
			}
			else if (cmd == "uciok") { 
				engine.handle_uciok(ss); 
			
				engine.send_isready();

				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
				);
			}
			else { 
				// Quietly ignore all other commands and continue reading  
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
				);
			}
		}

		void on_recv_launch2(UciEngine& engine, const boost::system::error_code& ec, size_t size)
		{
			// 1.) --- Get the line that we just read ---
			stringstream ss = engine.readLine();

			////cout << ss.str() << endl;

			// 2.) --- Extract the command token ---
			string cmd;
			ss >> cmd;

			// 3.) --- Determine how to handle the command ---
			if (cmd == "copyprotection") {
				engine.handle_copyprotection(ss);
				
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
				);
			}
			else if (cmd == "readyok") {
				engine.handle_readyok(ss);

				// Do not re-establish the callback. Simply return.
			}
			else {
				// Quietly ignore all other commands and continue reading  
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
				);
			}
		}

		void on_recv_eval(UciEngine& engine, const boost::system::error_code& ec, size_t size)
		{
			// 1.) --- Get the line that we just read ---
			stringstream ss = engine.readLine();

			////cout << ss.str() << endl;

			// 2.) --- Extract the command token ---
			string cmd;
			ss >> cmd;

			// 3.) --- Determine how to handle the command ---
			if (cmd == "info") {
				engine.handle_info(ss);

				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(engine, ec, size); }
				);
			}
			else if (cmd == "bestmove") {
				engine.handle_bestmove(ss);

				// Do not re-establish the callback. Simply return.
			}
			else {
				// Quietly ignore all other commands and continue reading  
				boost::asio::async_read_until(
					engine.pin,
					engine.buf_in,
					'\n',
					[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(engine, ec, size); }
				);
			}
		}

		// ------------------------ METHODS -----------------------------------------

		void UciEngine::launch()
		{
			// 1.) --- Search for the stockfish executable ---
			fs::path pathToEngine =
				bp::search_path("stockfish_13_win_x64_bmi2");
				//R"dil(C:\Program Files\stockfish_13_win_x64_bmi2\stockfish_13_win_x64_bmi2.exe)dil";

			launch(pathToEngine);
		}

		void UciEngine::launch(const string& engineName)
		{
			fs::path path = engineName;

			launch(path);
		}

		void UciEngine::launch(const boost::filesystem::path& path)
		{
			// 1.) --- Make sure file exists and is an executable ---
			if (fs::exists(path) == false) {
				throw exception("Could not open executable");
			}

			// 2.) --- Open as a child process ---
			m_child = bp::child(path, bp::std_in < pout, bp::std_out > pin);

			// 3.) --- Make sure chess engine is running ---
			if (m_child.running() == false) {
				throw exception("Could not run chess engine");
			}

			// 1.) --- Put engine in UCI mode ---
			this->send_uci();

			boost::asio::async_read_until(
				pin,
				buf_in,
				'\n',
				[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(*this, ec, size); }
			);

			ioc.run();	// blocking call
			ioc.reset();
		}

		int UciEngine::eval(const Position& pos)
		{
			// 1.) --- Tell engine to expect a new game ---
			send_ucinewgame();

			// 2.) --- Set options (optional) ---
			//send_setoption();
			//send_setoption();
			//send_setoption();

			// 3.) --- Set current position ---
			send_position(pos);

			// 3.) --- Start eval ---
			UciSearchCommands sCmd;
			sCmd.nodes = 100'000;
			send_go(sCmd);

			// *** Here engine will evaluate current position ***
			// And periodically print 'info' messages about its progress
			// When it finishes it will print 'bestmove <move> ...'
			// Warning: If engine was given the 'go infinite' or 'go' command
			//	we need to tell it to stop by issuing the 'stop' command

			// 4.) --- Wait till engine finishes ---
			boost::asio::async_read_until(
				pin,
				buf_in,
				'\n',
				[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(*this, ec, size); }
			);

			ioc.reset();
			ioc.run();	// blocking call
			ioc.reset();

			return bestMoveEval;	// Should be set from handle_bestmove
		}

		MoveList UciEngine::movegen(const Position& pos)
		{
			MoveList moves;

			// 1.) --- Tell engine to expect a new game ---
			send_ucinewgame();

			// 2.) --- Set options (Optional) ---
			send_setoption("MultiPV", "500");

			// 2.) --- Set current position ---
			send_position(pos);

			// 3.) --- Start Move Generation ---
			UciSearchCommands sCmd;
			sCmd.depth = 1;
			send_go(sCmd);

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
			send_quit();	// engine process should close on its own as soon as it can.

			// If engine doesn't close, the destructor will forcefully close it.
		}

		// ------------------------ UCI COMMANDS ------------------------------------

		// ------------------------ SEND COMMANDS -----------------------------------

		void UciEngine::send_uci()
		{
			pout << "uci" << endl;
		}

		void UciEngine::send_debug(bool isDebug)
		{
			pout << "debug " << (isDebug ? "on" : "off") << endl;
		}

		void UciEngine::send_isready()
		{
			pout << "isready" << endl;
		}

		void UciEngine::send_setoption(const string& opName, const string& opVal)
		{
			pout << "setoption name " << opName << " value " << opVal << endl;
		}

		void UciEngine::send_ucinewgame()
		{
			pout << "ucinewgame" << endl;
		}

		void UciEngine::send_position()
		{
			pout << "position startpos" << endl;
		}

		void UciEngine::send_position(const string& fen)
		{
			pout << "position " << fen << endl;
		}

		void UciEngine::send_position(const Position& pos)
		{
			pout << "position " << pos.toFEN() << endl;
		}

		void UciEngine::send_go()
		{
			pout << "go" << endl;
		}

		void UciEngine::send_go(const UciSearchCommands& sCMD)
		{
			pout << "go " << sCMD << endl;
		}

		void UciEngine::send_stop()
		{
			pout << "stop" << endl;
		}

		void UciEngine::send_ponderhit()
		{
			pout << "ponderhit" << endl;
		}

		void UciEngine::send_quit()
		{
			pout << "quit" << endl;
		}

		// ------------------------ HANDLERS ----------------------------------------

		void UciEngine::handle(std::istream& is)
		{
			string cmd;
			is >> cmd;

			if (cmd == "id")					handle_id(is);
			else if (cmd == "uciok")			handle_uciok(is);
			else if (cmd == "readyok")			handle_readyok(is);
			else if (cmd == "bestmove")			handle_bestmove(is);
			else if (cmd == "copyprotection")	handle_copyprotection(is);
			else if (cmd == "registration")		handle_registration(is);
			else if (cmd == "info")				handle_info(is);
			else if (cmd == "option")			handle_option(is);
			else {
#ifdef _DEBUG
				string rest;
				getline(is, rest);
				cout << "Command not recognized: " << cmd << ' ' << rest << endl;
#endif // _DEBUG
			}
		}

		void UciEngine::handle_id(std::istream& is)
		{
			string key;
			is >> key;

			string val;
			getline(is, val);	// get rest of line

			if (key == "name") {
				name = val;
			}
			else if (key == "author") {
				author = val;
			}
		}

		void UciEngine::handle_uciok(std::istream& is)
		{

		}

		void UciEngine::handle_readyok(std::istream& is) {}

		void UciEngine::handle_bestmove(std::istream& is)
		{
			string bestMove;
			string ponder;
			string ponderMove;

			is >> bestMove >> ponder >> ponderMove;	// TODO: Might be blocking if "ponder <move>" is not in `is`

			this->bestMove = Move{ bestMove };

			if (info.score.is_initialized()) {
				if (info.score->cp.is_initialized()) {
					this->bestMoveEval = info.score->cp.get();
				}
			}
		}

		void UciEngine::handle_copyprotection(std::istream& is) {}

		void UciEngine::handle_registration(std::istream& is) {}

		void UciEngine::handle_info(std::istream& is) 
		{
			is >> info;
		}

		void UciEngine::handle_option(std::istream& is) {
			//string str;
			//getline(is, str);
		}

		// Only called from handle_option()
		void UciEngine::handle_option_name(std::istream& is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_type(std::istream& is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_default(std::istream& is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_min(std::istream& is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_max(std::istream& is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_var(std::istream& is) {}

	} // namespace uci
} // namespace forge
