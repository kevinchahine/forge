#include "UciEngine.h"

#include <boost/asio/read.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/io.hpp>

#include <string>
#include <functional>

using namespace std;
namespace fs = boost::filesystem;
namespace bp = boost::process;

namespace forge
{
	namespace uci
	{
		// ------------------------ COROUTINES --------------------------------------

		void on_recv_launch1(UciEngine & engine, const boost::system::error_code& ec, size_t size)
		{
			cout << __FUNCTION__ << endl;

			// Tokenize command
			engine.streambufIn.commit(size + 1);

			// *** Here engine will: ***
			string cmd;
			engine.in >> cmd;
			cout << "cmd = " << cmd;
			string str;
			getline(engine.in, str);
			cout << str << endl;
			if (cmd == "id") {
				engine.handle_id(engine.in);

				boost::asio::async_read(
					engine.apIn,
					engine.streambuf_mbt_in,
					[&](const boost::system::error_code& ec, size_t size) {
					on_recv_launch1(engine, ec, size); });
			}
			else if (cmd == "option") {
				engine.handle_option(engine.in);
				
				boost::asio::async_read(
					engine.apIn,
					engine.streambuf_mbt_in,
					[&](const boost::system::error_code& ec, size_t size) {
					on_recv_launch1(engine, ec, size); });
			}
			else if (cmd == "uciok") {
				engine.handle_uciok(engine.in);
				// Don't re-establish on_recv_launch1 as a callback
				//send_setoption();
				// ...
				//send_setoption();
				
				engine.send_isready();

				boost::asio::async_read(
					engine.apIn,
					engine.streambuf_mbt_in,
					[&](const boost::system::error_code& ec, size_t size) {
					on_recv_launch2(engine, ec, size); });
			}
			else {
				// Ignore all other messages
				string str;
				getline(engine.in, str);
				cout << "else: " << str << endl;
				boost::asio::async_read(
					engine.apIn,
					engine.streambuf_mbt_in,
					[&](const boost::system::error_code& ec, size_t size) {
					on_recv_launch1(engine, ec, size); });
			}
		}

		void on_recv_launch2(UciEngine& engine, const boost::system::error_code& ec, size_t size)
		{
			// Tokenize command
			vector<string> tokens;
			tokens.push_back("Just to prevent errors");	// remove this line
			// tokenize(something)

			const string& cmd = tokens.front();
			if (cmd == "readyok") {
				//handle_readyok();
				// Do not establish any more callbacks. Handler chain is complete.
			}
			else {
				// Ignore all other messages 
				//context.post(on_recv_launch2);
			}
		}

		void on_recv_eval()
		{
			// Tokenize command
			vector<string> tokens;
			tokens.push_back("Just to prevent errors");	// remove this line
			// tokenize(something)

			const string& cmd = tokens.front();
			if (cmd == "info") {
				// handle_info();
				// context.post(on_recv_eval);
			}
			else if (cmd == "bestmove") {
				// handle_bestmove();
				// Do not establish any more callbacks. Handler chain is complete.
			}
			else {
				// Ignore all other messages 
				//context.post(on_recv_eval);
			}
		}

		// ------------------------ METHODS -----------------------------------------

		void UciEngine::launch()
		{
			// 1.) --- Search for the stockfish executable ---
			fs::path pathToEngine = 
				//bp::search_path();
				R"dil(C:\Program Files\stockfish_13_win_x64_bmi2\stockfish_13_win_x64_bmi2.exe)dil";
			
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
			m_child = bp::child(path, bp::std_in < pout, bp::std_out > apIn);

			// 3.) --- Make sure chess engine is running ---
			if (m_child.running() == false) {
				throw exception("Could not run chess engine");
			}

			// 1.) --- Put engine in UCI mode ---
			this->send_uci();

			boost::asio::async_read(
				apIn,
				streambuf_mbt_in,
				[&](const boost::system::error_code& ec, size_t size) { 
					on_recv_launch1(*this, ec, size); });

			cout << "Start running" << endl;
			context.run();
			cout << "Stop running" << endl;
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
			//context.post(on_recv_eval);
			context.run();

			int eval = 0; // ???

			return eval;
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

		void UciEngine::handle_id(std::istream & is) 
		{
			//string str;
			//getline(is, str);
		}

		void UciEngine::handle_uciok(std::istream & is) 
		{
			//string str;
			//getline(is, str);
		}

		void UciEngine::handle_readyok(std::istream & is) {}

		void UciEngine::handle_bestmove(std::istream & is) {}

		void UciEngine::handle_copyprotection(std::istream & is) {}

		void UciEngine::handle_registration(std::istream & is) {}

		void UciEngine::handle_info(std::istream & is) {}

		void UciEngine::handle_option(std::istream & is) {

			//string str;
			//getline(is, str);
		}

		// Only called from handle_option()
		void UciEngine::handle_option_name(std::istream & is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_type(std::istream & is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_default(std::istream & is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_min(std::istream & is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_max(std::istream & is) {}

		// Only called from handle_option()
		void UciEngine::handle_option_var(std::istream & is) {}

	} // namespace uci
} // namespace forge