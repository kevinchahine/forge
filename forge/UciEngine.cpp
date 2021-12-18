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

			// 4.) --- Up engine in UCI mode ---
			uci();

			// *** Here engine will: ***
			//	- switch to UCI mode
			//	- identify itself with the id message
			//	- list options that it supports (if any)
			//	- finish by replying "uciok"

			// 5.) --- 
		}

		int UciEngine::eval(const Position& pos)
		{
			return 0;
		}
		
		MoveList UciEngine::movegen(const Position& pos)
		{
			return MoveList();
		}

		void UciEngine::close()
		{
		}

		// ------------------------ UCI COMMANDS ------------------------------------
		
		void UciEngine::uci()
		{
			pout << "uci" << endl;
		}
		
	} // namespace uci
} // namespace forge