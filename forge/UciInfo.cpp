#include "UciInfo.h"

#include <boost/optional/optional_io.hpp>

#include <string>

// Inject stream insertion operator in std namespace
namespace std
{
	ostream& operator<<(ostream& os, const vector<forge::Move>& moves)
	{
		for (const auto& move : moves) {
			os << move << ' ';
		}

		return os;
	}

	istream& operator>>(istream& is, vector<forge::Move>& moves)
	{
		string token;

		//while (true) {
		//	cout << "token: " << token << " -> " << forge::Move{ token } << endl;
		//	moves.emplace_back(forge::Move{ token });
		//}

		return is;
	}
} // namespace std

using namespace std;

namespace forge
{
	namespace uci
	{
		std::ostream& operator<<(std::ostream& os, const UciInfo& info)
		{
			os << "info ";

			if (info.depth)				os << "depth " << info.depth.get() << ' ';
			if (info.seldepth)			os << "seldepth " << info.seldepth.get() << ' ';
			if (info.time)				os << "time " << info.time.get() << ' ';
			if (info.nodes)				os << "nodes " << info.nodes.get() << ' ';
			if (info.pv)				os << "pv " << info.pv.get() << ' ';
			if (info.multipv)			os << "multipv " << info.multipv.get() << ' ';
			if (info.score)				os << info.score.get();
			if (info.currmove)			os << "currmove " << info.currmove.get() << ' ';
			if (info.currmovenumber)	os << "currmovenumber " << info.currmovenumber.get() << ' ';
			if (info.hashfull)			os << "hashfull " << info.hashfull.get() << ' ';
			if (info.nps)				os << "nps " << info.nps.get() << ' ';
			if (info.tbhits)			os << "tbhits " << info.tbhits.get() << ' ';
			if (info.sbhits)			os << "sbhits " << info.sbhits.get() << ' ';
			if (info.cpuload)			os << "cpuload " << info.cpuload.get() << ' ';
			if (info.refutation)		os << "refutation " << info.refutation.get() << ' ';
			if (info.currline)			os << info.currline.get() << ' ';
			if (info.string)			os << "string " << info.string.get() << ' ';	// must be last

			return os;
		}

		std::istream& operator>>(std::istream& is, UciInfo& info)
		{
			string token;

			while (true) {
				// --- extract info key ---
				is >> token;
				
				// --- extract info value (based on the key) ---
				/*if (token == "info");	// ignore. we already know this is an info message
				else*/if (token == "depth")			is >> info.depth;
				else if (token == "seldepth")		is >> info.seldepth;
				else if (token == "time")			is >> info.time;
				else if (token == "nodes")			is >> info.nodes;
				else if (token == "pv")				is >> info.pv;
				else if (token == "multipv")		is >> info.multipv;
				else if (token == "score")			is >> info.score;
				else if (token == "currmove")		is >> info.currmove;
				else if (token == "currmovenumber")	is >> info.currmovenumber;
				else if (token == "hashfull")		is >> info.hashfull;
				else if (token == "nps")			is >> info.nps;
				else if (token == "tbhits")			is >> info.tbhits;
				else if (token == "sbhits")			is >> info.sbhits;
				else if (token == "cpuload")		is >> info.cpuload;
				else if (token == "refutation")		is >> info.refutation;
				else if (token == "currline")		is >> info.currline;
				else if (token == "string")			is >> info.string;			// must be last
			}

			return is;
		}
	} // namespace uci
} // namespace forge