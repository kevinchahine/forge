#pragma once

#include <boost/process/child.hpp>
#include <boost/filesystem/path.hpp>

#include "UciBase.h"
#include "Position.h"
#include "MoveList.h"
#include "UciSearchCommands.h"

namespace forge
{
	namespace uci
	{
		// An interface to communicate with a UCI compatible chess engine
		// running as a child process.
		// Sends messages to the engine
		// Receives messages from the engine
		class UciEngine : public UciBase
		{
		public:
			UciEngine() = default;
			virtual ~UciEngine() noexcept { close(); }

			// Attempts to start a chess engine. Searches for UCI compatible chess engine
			// Currently only searches for stockfish_13_win_x64_bmi2.exe
			// default: stockfish_13_win_x64_bmi2.exe
			// throws an exception upon failure.
			void launch();

			// See comments of launch()
			// engineName - name of a chessengine (case insensitive)
			// Can be:
			//	- "stockfish"
			//	- "lc0"
			//	- "forge"
			void launch(const std::string& engineName);

			// See comments of launch()
			// path - path to a chess engine executable
			void launch(const boost::filesystem::path& path);

			// Calls on engine to evaluate Position `pos`.
			// The call is blocking and waits for engine to respond with the evaluation results
			// evaluation is returned in units of centi pawns
			int eval(const Position& pos);

			// Calls on engine to generate legal moves from Position `pos`
			// The call is blocking and waits for engine to respond with a list of legal moves
			MoveList movegen(const Position& pos);

			// Closes the chess engine program
			// Calling this method is optional because it will be called from
			// the destructor.
			void close();

		public:	// ---------------- UCI COMMANDS ------------------------------------

			// ----------------------- SEND COMMANDS -----------------------------------

			void send_uci();						
			
			void send_debug(bool isDebug);		
			
			void send_isready();					
			
			// TODO: there is more to options than just this. See UCI Definition
			void send_setoption(const std::string & opName, const std::string & opVal);
			
			void send_ucinewgame();				
			
			void send_position();				
			
			void send_position(const std::string& fen);

			void send_position(const Position& pos);

			// C - container storing type forge::Move
			// example: vector<Move>, list<Move>, ...
			template <typename C>
			void send_position(const std::string fen, const C & moves);	

			// C - container storing type forge::Move
			// example: vector<Move>, list<Move>, ...
			template <typename C>
			void send_position(const Position& pos, const C & moves);
			
			// Tells engine to search the position until the "stop" command is issued
			// Same as "go infinite"
			// Non-blocking call.
			void send_go();						
			
			// Non-blocking call.
			void send_go(const UciSearchCommands& sCMD);
			
			void send_stop();					
			
			void send_ponderhit();				
			
			void send_quit();					

			// ----------------------- HANDLERS ----------------------------------------

			void handle(std::istream & is);

			void handle_id(std::istream & is);

			void handle_uciok(std::istream & is);

			void handle_readyok(std::istream & is);

			void handle_bestmove(std::istream & is);

			void handle_copyprotection(std::istream & is);

			void handle_registration(std::istream & is);

			void handle_info(std::istream & is);

			void handle_option(std::istream & is);

			// Only called from handle_option()
			void handle_option_name(std::istream & is);

			// Only called from handle_option()
			void handle_option_type(std::istream & is);

			// Only called from handle_option()
			void handle_option_default(std::istream & is);

			// Only called from handle_option()
			void handle_option_min(std::istream & is);

			// Only called from handle_option()
			void handle_option_max(std::istream & is);

			// Only called from handle_option()
			void handle_option_var(std::istream & is);

			// ----------------------- WAIT METHODS ------------------------------------
			// These are blocking methods to to wait until a specific command is received
			// or a certain event occures. They can be used to turn an asynchronous 
			// operation into a synchronous one.
			// Calling these methods do not stop the object from reading from the 
			

		private:
			boost::process::child m_child;

			// --- Data about the engine ---
			std::string name;
			std::string author;

			Move bestMove;
			int bestMoveEval;
		};
		
		template<typename C>
		void UciEngine::send_position(const std::string fen, const C& moves)
		{
			pout << "position " << fen << " moves ";

			for (const Move& m : moves) {
				pout << m << ' ';
			}

			pout << endl;
		}
		
		template<typename C>
		void UciEngine::send_position(const Position& pos, const C& moves)
		{
			position(pos.toFEN(), moves);
		}
	} // namespace uci
} // namespace forge