#pragma once

//pthread_error #include <boost/process/child.hpp>
//pthread_error #include <boost/filesystem/path.hpp>

#include "UciBase.h"
#include "UciInfo.h"
#include "UciSearchCommands.h"
#include "Position.h"
#include "MoveList.h"

namespace forge
{
	namespace uci
	{
		// This class is, an interface to communicate with a UCI compatible chess engine.
		// This class will open a chess engine as a child process, and interface with
		// it using IO pipes. When a UciEngine object is destroyed, the chess engine
		// process is closed automatically. 
		// Sends messages to the engine that runs as a child process.
		// This class is not intended to interface with a UCI GUI. 
		// For that, use class UciGui instead.
		class UciEngine : public UciBase
		{
//pthread_error 		public:
//pthread_error 			UciEngine() = default;
//pthread_error 			virtual ~UciEngine() noexcept { close(); }
//pthread_error 
//pthread_error 			// Attempts to start a chess engine. Searches for UCI compatible chess engine
//pthread_error 			// Currently only searches for stockfish_13_win_x64_bmi2.exe
//pthread_error 			// default: stockfish_13_win_x64_bmi2.exe
//pthread_error 			// throws an exception upon failure.
//pthread_error 			void launch();
//pthread_error 
//pthread_error 			// See comments of launch()
//pthread_error 			// engineName - name of a chessengine (case insensitive)
//pthread_error 			// Can be:
//pthread_error 			//	- "stockfish"
//pthread_error 			//	- "lc0"
//pthread_error 			//	- "forge"
//pthread_error 			void launch(const std::string& engineName);
//pthread_error 
//pthread_error 			// See comments of launch()
//pthread_error 			// path - path to a chess engine executable
//pthread_error 			void launch(const boost::filesystem::path& path);
//pthread_error 
//pthread_error 			// Calls on engine to evaluate Position `pos`.
//pthread_error 			// The call is blocking and waits for engine to respond with the evaluation results
//pthread_error 			// evaluation is returned in units of centi pawns
//pthread_error 			int eval(const Position& pos);
//pthread_error 
//pthread_error 			// Calls on engine to generate legal moves from Position `pos`
//pthread_error 			// The call is blocking and waits for engine to respond with a list of legal moves
//pthread_error 			MoveList movegen(const Position& pos);
//pthread_error 
//pthread_error 			// Closes the chess engine program
//pthread_error 			// Calling this method is optional because it will be called from
//pthread_error 			// the destructor.
//pthread_error 			void close();
//pthread_error 
//pthread_error 		public:	// ---------------- UCI COMMANDS ------------------------------------
//pthread_error 
//pthread_error 			// ----------------------- SEND COMMANDS -----------------------------------
//pthread_error 
//pthread_error 			void send_uci();						
//pthread_error 			
//pthread_error 			void send_debug(bool isDebug);		
//pthread_error 			
//pthread_error 			void send_isready();					
//pthread_error 			
//pthread_error 			// TODO: there is more to options than just this. See UCI Definition
//pthread_error 			void send_setoption(const std::string & opName, const std::string & opVal);
//pthread_error 			
//pthread_error 			void send_ucinewgame();				
//pthread_error 			
//pthread_error 			void send_position();				
//pthread_error 			
//pthread_error 			void send_position(const std::string& fen);
//pthread_error 
//pthread_error 			void send_position(const Position& pos);
//pthread_error 
//pthread_error 			// C - container storing type forge::Move
//pthread_error 			// example: vector<Move>, list<Move>, ...
//pthread_error 			template <typename C>
//pthread_error 			void send_position(const std::string fen, const C & moves);	
//pthread_error 
//pthread_error 			// C - container storing type forge::Move
//pthread_error 			// example: vector<Move>, list<Move>, ...
//pthread_error 			template <typename C>
//pthread_error 			void send_position(const Position& pos, const C & moves);
//pthread_error 			
//pthread_error 			// Tells engine to search the position until the "stop" command is issued
//pthread_error 			// Same as "go infinite"
//pthread_error 			// Non-blocking call.
//pthread_error 			void send_go();						
//pthread_error 			
//pthread_error 			// Non-blocking call.
//pthread_error 			void send_go(const UciSearchCommands& sCMD);
//pthread_error 			
//pthread_error 			void send_stop();					
//pthread_error 			
//pthread_error 			void send_ponderhit();				
//pthread_error 			
//pthread_error 			void send_quit();					
//pthread_error 
//pthread_error 			// ----------------------- HANDLERS ----------------------------------------
//pthread_error 
//pthread_error 			void handle(std::istream & is);
//pthread_error 
//pthread_error 			void handle_id(std::istream & is);
//pthread_error 
//pthread_error 			void handle_uciok(std::istream & is);
//pthread_error 
//pthread_error 			void handle_readyok(std::istream & is);
//pthread_error 
//pthread_error 			void handle_bestmove(std::istream & is);
//pthread_error 
//pthread_error 			void handle_copyprotection(std::istream & is);
//pthread_error 
//pthread_error 			void handle_registration(std::istream & is);
//pthread_error 
//pthread_error 			void handle_info(std::istream & is);
//pthread_error 
//pthread_error 			void handle_option(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_name(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_type(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_default(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_min(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_max(std::istream & is);
//pthread_error 
//pthread_error 			// Only called from handle_option()
//pthread_error 			void handle_option_var(std::istream & is);
//pthread_error 
//pthread_error 			// ----------------------- WAIT METHODS ------------------------------------
//pthread_error 			// These are blocking methods to to wait until a specific command is received
//pthread_error 			// or a certain event occures. They can be used to turn an asynchronous 
//pthread_error 			// operation into a synchronous one.
//pthread_error 			// Calling these methods do not stop the object from reading from the 
//pthread_error 			
//pthread_error 
//pthread_error 		private:
//pthread_error 			boost::process::child m_child;
//pthread_error 
//pthread_error 			// --- Data about the engine ---
//pthread_error 			std::string name;
//pthread_error 			std::string author;
//pthread_error 
//pthread_error 			Move bestMove;
//pthread_error 			int bestMoveEval;
//pthread_error 			UciInfo info;
		};
		
//pthread_error 		template<typename C>
//pthread_error 		void UciEngine::send_position(const std::string fen, const C& moves)
//pthread_error 		{
//pthread_error 			pout << "position " << fen << " moves ";
//pthread_error 
//pthread_error 			for (const Move& m : moves) {
//pthread_error 				pout << m << ' ';
//pthread_error 			}
//pthread_error 
//pthread_error 			pout << endl;
//pthread_error 		}
//pthread_error 		
//pthread_error 		template<typename C>
//pthread_error 		void UciEngine::send_position(const Position& pos, const C& moves)
//pthread_error 		{
//pthread_error 			position(pos.toFEN(), moves);
//pthread_error 		}
	} // namespace uci
} // namespace forge