#pragma once

#include "UciBase.h"
#include "Position.h"
#include "MoveList.h"

#include <boost/process/child.hpp>
#include <boost/process/pipe.hpp>
#include <boost/filesystem/path.hpp>

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
			UciEngine() : UciBase(pin, pout) {}
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

			void uci();						
			
			void debug(bool isDebug);		
			
			void isready();					
			
			void setOption(const std::string & opName, const std::string & opVal);	
			
			//void setOptionOp1(const std::string& opNameopVal);
			
			//void setOptionOp2(const std::string& opNameopVal);
			
			// ...
			
			void registerEngine(const std::string& name, const std::string& val);
			
			void registerLater();			
			
			void registerName(const std::string & name);		
			
			void registerCode(const std::string & code);		
			
			void ucinewgame();				
			
			void position();				
			
			// C - container storing type forge::Move
			// example: vector<Move>, list<Move>, ...
			template <typename C>
			void position(const std::string fen, const C & moves);	

			// C - container storing type forge::Move
			// example: vector<Move>, list<Move>, ...
			template <typename C>
			void position(const Position& pos, const C & moves);
			
			void go();						
			
			//void go(const UciSearchCommands& sCMD);
			
			void stop();					
			
			void ponderhit();				
			
			void quit();					

		private:
			boost::process::child m_child;
			boost::process::ipstream pin;
			boost::process::opstream pout;
		};
	} // namespace uci
} // namespace forge