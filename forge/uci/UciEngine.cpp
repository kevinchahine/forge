#include "UciEngine.h"

//pthread_error #include <boost/asio/read_until.hpp>
//pthread_error #include <boost/bind.hpp>
//pthread_error #include <boost/process/search_path.hpp>
//pthread_error #include <boost/process/io.hpp>
//pthread_error #include <boost/optional/optional_io.hpp>

#include <string>
#include <functional>
#include <chrono>

using namespace std;
//pthread_error namespace fs = boost::filesystem;
//pthread_error namespace bp = boost::process;

namespace forge
{
	namespace uci
	{
//pthread_error 		// ------------------------ HANDLER CHAINS ----------------------------------
//pthread_error 
//pthread_error 		void on_recv_launch1(UciEngine& engine, const boost::system::error_code& ec, size_t size);
//pthread_error 		void on_recv_launch2(UciEngine& engine, const boost::system::error_code& ec, size_t size);
//pthread_error 		
//pthread_error 		// TODO: Reimplement these callback functions as classes with functors.
//pthread_error 		// Then make a single template function for async_read_until in such a way that we can 
//pthread_error 		// do:
//pthread_error 		//	async_read_until<OnRecvLaunch1>(engine);
//pthread_error 		// --- instead of ---
//pthread_error 		// boost::asio::async_read_until(
//pthread_error 		//	engine.pin,
//pthread_error 		// 		engine.buf_in,
//pthread_error 		// 		'\n',
//pthread_error 		// 		[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
//pthread_error 		//	);
//pthread_error 
//pthread_error 		void on_recv_launch1(UciEngine& engine, const boost::system::error_code& ec, size_t size)
//pthread_error 		{
//pthread_error 			// 1.) --- Get the line that we just read ---
//pthread_error 			stringstream ss = engine.readLine();
//pthread_error 
//pthread_error 			////cout << ss.str() << endl;
//pthread_error 
//pthread_error 			// 2.) --- Extract the command token ---
//pthread_error 			string cmd;
//pthread_error 			ss >> cmd;
//pthread_error 
//pthread_error 			// 3.) --- Determine how to handle the command ---
//pthread_error 
//pthread_error 			if (cmd == "id") { 
//pthread_error 				engine.handle_id(ss); 
//pthread_error 			
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 			else if (cmd == "option") { 
//pthread_error 				engine.handle_option(ss); 
//pthread_error 				
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 			else if (cmd == "uciok") { 
//pthread_error 				engine.handle_uciok(ss); 
//pthread_error 			
//pthread_error 				engine.send_isready();
//pthread_error 
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 			else { 
//pthread_error 				// Quietly ignore all other commands and continue reading  
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		void on_recv_launch2(UciEngine& engine, const boost::system::error_code& ec, size_t size)
//pthread_error 		{
//pthread_error 			// 1.) --- Get the line that we just read ---
//pthread_error 			stringstream ss = engine.readLine();
//pthread_error 
//pthread_error 			////cout << ss.str() << endl;
//pthread_error 
//pthread_error 			// 2.) --- Extract the command token ---
//pthread_error 			string cmd;
//pthread_error 			ss >> cmd;
//pthread_error 
//pthread_error 			// 3.) --- Determine how to handle the command ---
//pthread_error 			if (cmd == "copyprotection") {
//pthread_error 				engine.handle_copyprotection(ss);
//pthread_error 				
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 			else if (cmd == "readyok") {
//pthread_error 				engine.handle_readyok(ss);
//pthread_error 
//pthread_error 				// Do not re-establish the callback. Simply return.
//pthread_error 			}
//pthread_error 			else {
//pthread_error 				// Quietly ignore all other commands and continue reading  
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_launch2(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		void on_recv_eval(UciEngine& engine, const boost::system::error_code& ec, size_t size)
//pthread_error 		{
//pthread_error 			// 1.) --- Get the line that we just read ---
//pthread_error 			stringstream ss = engine.readLine();
//pthread_error 
//pthread_error 			////cout << ss.str() << endl;
//pthread_error 
//pthread_error 			// 2.) --- Extract the command token ---
//pthread_error 			string cmd;
//pthread_error 			ss >> cmd;
//pthread_error 
//pthread_error 			// 3.) --- Determine how to handle the command ---
//pthread_error 			if (cmd == "info") {
//pthread_error 				engine.handle_info(ss);
//pthread_error 
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 			else if (cmd == "bestmove") {
//pthread_error 				engine.handle_bestmove(ss);
//pthread_error 
//pthread_error 				// Do not re-establish the callback. Simply return.
//pthread_error 			}
//pthread_error 			else {
//pthread_error 				// Quietly ignore all other commands and continue reading  
//pthread_error 				boost::asio::async_read_until(
//pthread_error 					engine.pin,
//pthread_error 					engine.buf_in,
//pthread_error 					'\n',
//pthread_error 					[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(engine, ec, size); }
//pthread_error 				);
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		// ------------------------ METHODS -----------------------------------------
//pthread_error 
//pthread_error 		void UciEngine::launch()
//pthread_error 		{
//pthread_error 			// 1.) --- Search for the stockfish executable ---
//pthread_error 			fs::path pathToEngine =
//pthread_error 				bp::search_path("stockfish_13_win_x64_bmi2");
//pthread_error 				//R"dil(C:\Program Files\stockfish_13_win_x64_bmi2\stockfish_13_win_x64_bmi2.exe)dil";
//pthread_error 
//pthread_error 			launch(pathToEngine);
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::launch(const string& engineName)
//pthread_error 		{
//pthread_error 			fs::path path = engineName;
//pthread_error 
//pthread_error 			launch(path);
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::launch(const boost::filesystem::path& path)
//pthread_error 		{
//pthread_error 			// 1.) --- Make sure file exists and is an executable ---
//pthread_error 			if (fs::exists(path) == false) {
//pthread_error 				throw runtime_error("Could not open executable");
//pthread_error 			}
//pthread_error 
//pthread_error 			// 2.) --- Open as a child process ---
//pthread_error 			m_child = bp::child(path, bp::std_in < pout, bp::std_out > pin);
//pthread_error 
//pthread_error 			// 3.) --- Make sure chess engine is running ---
//pthread_error 			if (m_child.running() == false) {
//pthread_error 				throw runtime_error("Could not run chess engine");
//pthread_error 			}
//pthread_error 
//pthread_error 			// 1.) --- Put engine in UCI mode ---
//pthread_error 			this->send_uci();
//pthread_error 
//pthread_error 			boost::asio::async_read_until(
//pthread_error 				pin,
//pthread_error 				buf_in,
//pthread_error 				'\n',
//pthread_error 				[&](const boost::system::error_code& ec, size_t size) { on_recv_launch1(*this, ec, size); }
//pthread_error 			);
//pthread_error 
//pthread_error 			ioc.run();	// blocking call
//pthread_error 			ioc.reset();
//pthread_error 		}
//pthread_error 
//pthread_error 		int UciEngine::eval(const Position& pos)
//pthread_error 		{
//pthread_error 			// 1.) --- Tell engine to expect a new game ---
//pthread_error 			send_ucinewgame();
//pthread_error 
//pthread_error 			// 2.) --- Set options (optional) ---
//pthread_error 			//send_setoption();
//pthread_error 			//send_setoption();
//pthread_error 			//send_setoption();
//pthread_error 
//pthread_error 			// 3.) --- Set current position ---
//pthread_error 			send_position(pos);
//pthread_error 
//pthread_error 			// 3.) --- Start eval ---
//pthread_error 			UciSearchCommands sCmd;
//pthread_error 			sCmd.nodes = 100'000;
//pthread_error 			sCmd.depth = 5;
//pthread_error 			send_go(sCmd);
//pthread_error 
//pthread_error 			// *** Here engine will evaluate current position ***
//pthread_error 			// And periodically print 'info' messages about its progress
//pthread_error 			// When it finishes it will print 'bestmove <move> ...'
//pthread_error 			// Warning: If engine was given the 'go infinite' or 'go' command
//pthread_error 			//	we need to tell it to stop by issuing the 'stop' command
//pthread_error 
//pthread_error 			// 4.) --- Wait till engine finishes ---
//pthread_error 			boost::asio::async_read_until(
//pthread_error 				pin,
//pthread_error 				buf_in,
//pthread_error 				'\n',
//pthread_error 				[&](const boost::system::error_code& ec, size_t size) { on_recv_eval(*this, ec, size); }
//pthread_error 			);
//pthread_error 
//pthread_error 			ioc.reset();
//pthread_error 			ioc.run();	// blocking call
//pthread_error 			ioc.reset();
//pthread_error 
//pthread_error 			return bestMoveEval;	// Should be set from handle_bestmove
//pthread_error 		}
//pthread_error 
//pthread_error 		MoveList UciEngine::movegen(const Position& pos)
//pthread_error 		{
//pthread_error 			MoveList moves;
//pthread_error 
//pthread_error 			// 1.) --- Tell engine to expect a new game ---
//pthread_error 			send_ucinewgame();
//pthread_error 
//pthread_error 			// 2.) --- Set options (Optional) ---
//pthread_error 			send_setoption("MultiPV", "500");
//pthread_error 
//pthread_error 			// 2.) --- Set current position ---
//pthread_error 			send_position(pos);
//pthread_error 
//pthread_error 			// 3.) --- Start Move Generation ---
//pthread_error 			UciSearchCommands sCmd;
//pthread_error 			sCmd.depth = 1;
//pthread_error 			send_go(sCmd);
//pthread_error 
//pthread_error 			// *** Here engine will evaluate current position ***
//pthread_error 			// And periodically print 'info' messages about its progress
//pthread_error 			// When it finishes it will print 'bestmove <move> ...'
//pthread_error 			// Warning: If engine was given the 'go infinite' or 'go' command
//pthread_error 			//	we need to tell it to stop by issuing the 'stop' command
//pthread_error 
//pthread_error 			// 4.) --- Wait till engine finishes ---
//pthread_error 			// int eval = readEval();
//pthread_error 
//pthread_error 			return moves;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::close()
//pthread_error 		{
//pthread_error 			send_quit();	// engine process should close on its own as soon as it can.
//pthread_error 
//pthread_error 			// If engine doesn't close, the destructor will forcefully close it.
//pthread_error 		}
//pthread_error 
//pthread_error 		// ------------------------ UCI COMMANDS ------------------------------------
//pthread_error 
//pthread_error 		// ------------------------ SEND COMMANDS -----------------------------------
//pthread_error 
//pthread_error 		void UciEngine::send_uci()
//pthread_error 		{
//pthread_error 			pout << "uci" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_debug(bool isDebug)
//pthread_error 		{
//pthread_error 			pout << "debug " << (isDebug ? "on" : "off") << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_isready()
//pthread_error 		{
//pthread_error 			pout << "isready" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_setoption(const string& opName, const string& opVal)
//pthread_error 		{
//pthread_error 			pout << "setoption name " << opName << " value " << opVal << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_ucinewgame()
//pthread_error 		{
//pthread_error 			pout << "ucinewgame" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_position()
//pthread_error 		{
//pthread_error 			pout << "position startpos" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_position(const string& fen)
//pthread_error 		{
//pthread_error 			pout << "position " << fen << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_position(const Position& pos)
//pthread_error 		{
//pthread_error 			pout << "position fen " << pos.toFEN() << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_go()
//pthread_error 		{
//pthread_error 			pout << "go" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_go(const UciSearchCommands& sCMD)
//pthread_error 		{
//pthread_error 			pout << "go " << sCMD << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_stop()
//pthread_error 		{
//pthread_error 			pout << "stop" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_ponderhit()
//pthread_error 		{
//pthread_error 			pout << "ponderhit" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::send_quit()
//pthread_error 		{
//pthread_error 			pout << "quit" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		// ------------------------ HANDLERS ----------------------------------------
//pthread_error 
//pthread_error 		void UciEngine::handle(std::istream& is)
//pthread_error 		{
//pthread_error 			string cmd;
//pthread_error 			is >> cmd;
//pthread_error 
//pthread_error 			if (cmd == "id")					handle_id(is);
//pthread_error 			else if (cmd == "uciok")			handle_uciok(is);
//pthread_error 			else if (cmd == "readyok")			handle_readyok(is);
//pthread_error 			else if (cmd == "bestmove")			handle_bestmove(is);
//pthread_error 			else if (cmd == "copyprotection")	handle_copyprotection(is);
//pthread_error 			else if (cmd == "registration")		handle_registration(is);
//pthread_error 			else if (cmd == "info")				handle_info(is);
//pthread_error 			else if (cmd == "option")			handle_option(is);
//pthread_error 			else {
//pthread_error #ifdef _DEBUG
//pthread_error 				string rest;
//pthread_error 				getline(is, rest);
//pthread_error 				cout << "Command not recognized: " << cmd << ' ' << rest << endl;
//pthread_error #endif // _DEBUG
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::handle_id(std::istream& is)
//pthread_error 		{
//pthread_error 			string key;
//pthread_error 			is >> key;
//pthread_error 
//pthread_error 			string val;
//pthread_error 			getline(is, val);	// get rest of line
//pthread_error 
//pthread_error 			if (key == "name") {
//pthread_error 				name = val;
//pthread_error 			}
//pthread_error 			else if (key == "author") {
//pthread_error 				author = val;
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::handle_uciok(std::istream& is)
//pthread_error 		{
//pthread_error 
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::handle_readyok(std::istream& is) {}
//pthread_error 
//pthread_error 		void UciEngine::handle_bestmove(std::istream& is)
//pthread_error 		{
//pthread_error 			string bestMove;
//pthread_error 			string ponder;
//pthread_error 			string ponderMove;
//pthread_error 
//pthread_error 			is >> bestMove >> ponder >> ponderMove;	// TODO: Might be blocking if "ponder <move>" is not in `is`
//pthread_error 
//pthread_error 			this->bestMove = Move{ bestMove };
//pthread_error 
//pthread_error 			cout << "Best move:" << this->bestMove << endl
//pthread_error 				<< "Info: " << info << endl;
//pthread_error 			
//pthread_error 			if (info.score.is_initialized()) {
//pthread_error 				if (info.score->cp.is_initialized()) {
//pthread_error 					this->bestMoveEval = info.score->cp.get();
//pthread_error 				}
//pthread_error 			}
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::handle_copyprotection(std::istream& is) {}
//pthread_error 
//pthread_error 		void UciEngine::handle_registration(std::istream& is) {}
//pthread_error 
//pthread_error 		void UciEngine::handle_info(std::istream& is) 
//pthread_error 		{
//pthread_error 			is >> info;
//pthread_error 
//pthread_error 			cout << info << endl;
//pthread_error 			
//pthread_error 			if (info.score.is_initialized() && info.score->cp.is_initialized())
//pthread_error 				cout << "score: " << info.score->cp.get() << " cp" << endl;
//pthread_error 		}
//pthread_error 
//pthread_error 		void UciEngine::handle_option(std::istream& is) {
//pthread_error 			//string str;
//pthread_error 			//getline(is, str);
//pthread_error 		}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_name(std::istream& is) {}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_type(std::istream& is) {}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_default(std::istream& is) {}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_min(std::istream& is) {}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_max(std::istream& is) {}
//pthread_error 
//pthread_error 		// Only called from handle_option()
//pthread_error 		void UciEngine::handle_option_var(std::istream& is) {}

	} // namespace uci
} // namespace forge
