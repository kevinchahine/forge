#pragma once

#include "Move.h"

#include <boost/optional/optional.hpp>

#include <vector>

namespace forge
{
	namespace uci
	{
		class UciSearchCommands
		{
		public:
			friend std::ostream& operator<<(std::ostream& os, const UciSearchCommands & sCmd);

		public:
			boost::optional<std::vector<Move>> searchMoves;	// restricts search to these moves
			boost::optional<bool> ponder;					// execute search in "ponder" mode
			boost::optional<int> wtime; 					// how many msec white has on the clock
			boost::optional<int> btime; 					// how many msec white has on the clock
			boost::optional<int> winc;						// whites increment per move in msec
			boost::optional<int> binc; 						// blacks increment per move in msec
			boost::optional<int> movestogo;					// # of moves to next time control
			boost::optional<int> depth;						// search `depth` plies only 
			boost::optional<int> nodes;						// search `nodes` nodes only
			boost::optional<int> mate;						// search for a mate in x moves 
			boost::optional<int> movetime;					// search exactly `movetime` msecs 
			boost::optional<bool> infinite;					// search until "stop" command. Do not exit search until "stop" cmd is issued
		};
	} // namespace uci
} // namespace forge