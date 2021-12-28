#pragma once

#include "Move.h"
#include "UciScore.h"
#include "UciCurrLine.h"

#include <boost/optional/optional.hpp>

#include <vector>

namespace forge
{
	namespace uci
	{
		class UciInfo
		{
		public:
			friend std::ostream & operator<<(std::ostream & os, const UciInfo & info);

			// Reads until end of line
			friend std::istream & operator>>(std::istream & is, UciInfo & info);

		public:
			// search depth in plies
			boost::optional<int> depth;
			// selective search depth in plies,
			// if the engine sends seldepth there must also be a "depth" present in the same string.
			boost::optional<int> seldepth;
			// the time searched in ms, this should be sent together with the pv.
			boost::optional<int> time;
			// x nodes searched, the engine should send this info regularly
			boost::optional<int> nodes;
			// the best line found
			boost::optional<std::vector<Move>> pv;
			// this for the multi pv mode.
			// for the best move / pv add "multipv 1" in the string when you send the pv.
			// in k - best mode always send all k variants in k strings together.
			boost::optional<int> multipv;
			boost::optional<UciScore> score;
			// currently searching this move
			boost::optional<Move> currmove;
			// currently searching move number x, for the first move x should be 1 not 0.
			boost::optional<int> currmovenumber;
			// the hash is x permill full, the engine should send this info regularly
			boost::optional<int> hashfull;
			// x nodes per second searched, the engine should send this info regularly
			boost::optional<int> nps;
			// x positions where found in the endgame table bases
			boost::optional<int> tbhits;
			// x positions where found in the shredder endgame databases
			boost::optional<int> sbhits;
			// the cpu usage of the engine is x permill.
			boost::optional<int> cpuload;
			// move <move1> is refuted by the line <move2> ... <movei>, i can be any number >= 1.
			// Example: after move d1h5 is searched, the engine can send
			//	"info refutation d1h5 g6h5"
			// if g6h5 is the best answer after d1h5 or if g6h5 refutes the move d1h5.
			// if there is no refutation for d1h5 found, the engine should just send
			// "info refutation d1h5"
			// The engine should only send this if the option "UCI_ShowRefutations" is set to true.
			boost::optional<std::vector<Move>> refutation;
			// this is the current line the engine is calculating. <cpunr> is the number of the cpu if
			// the engine is running on more than one cpu. <cpunr> = 1,2,3....
			// if the engine is just using one cpu, <cpunr> can be omitted.
			// If <cpunr> is greater than 1, always send all k lines in k strings together.
			// The engine should only send this if the option "UCI_ShowCurrLine" is set to true.
			boost::optional<UciCurrLine> currline;
			// any string str which will be displayed be the engine,
			// if there is a string command the rest of the line will be interpreted as <str>.
			boost::optional<std::string> string;
		};
	} // namespace uci
} // namespace forge