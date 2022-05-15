#include "UciSearchCommands.h"

using namespace std;

namespace forge
{
	namespace uci
	{
		std::ostream& operator<<(std::ostream& os, const UciSearchCommands& sCmd)
		{
			if (sCmd.searchMoves && sCmd.searchMoves->size()) {
				os << "searchmoves ";

				for (const Move& m : sCmd.searchMoves.get()) {
					os << m << ' ';
				}
			}

			if (sCmd.ponder)	os << "ponder ";
			if (sCmd.wtime)		os << "wtime " << sCmd.wtime.get() << ' ';
			if (sCmd.btime)		os << "btime " << sCmd.btime.get() << ' ';
			if (sCmd.winc)		os << "winc " << sCmd.winc.get() << ' ';
			if (sCmd.binc)		os << "binc " << sCmd.binc.get() << ' ';
			if (sCmd.movestogo)	os << "movestogo " << sCmd.movestogo.get() << ' ';	
			if (sCmd.depth)		os << "depth " << sCmd.depth.get() << ' ';		
			if (sCmd.nodes)		os << "nodes " << sCmd.nodes.get() << ' ';		
			if (sCmd.mate)		os << "mate " << sCmd.mate.get() << ' ';		
			if (sCmd.movetime)	os << "movetime " << sCmd.movetime.get() << ' ';	
			if (sCmd.infinite)	os << "infinite ";	

			return os;
		}
	} // namespace uci
} // namespace forge