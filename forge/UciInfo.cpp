#include "UciInfo.h"

#include <boost/optional/optional_io.hpp>

#include <string>
#include <sstream>
#include <queue>		// for priority_queue
#include <vector>
#include <functional>	// std::greater

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
		while (is.eof() == false) {
			string token;
			
			is >> token;
			
			if (token.size()) {
				moves.emplace_back(token);
			}
		}

		return is;
	}
} // namespace std

using namespace std;

namespace forge
{
	namespace uci
	{
		// Searchs `line` for `keyword` and pushes its index into `keywords`.
		// If `keyword` is not found, then its value will not be pushed into `keywords`.
		// Keywords found after `end` will not be pushed either. 
		// PRIORITY_QUEUE_T must be a priority_queue<size_t> or similar
		template<class PRIORITY_QUEUE_T>
		void findAndPushKeyword(PRIORITY_QUEUE_T& keywords, const string& line, const string& keyword, size_t end)
		{
			size_t pos = line.find(keyword);

			if (pos <= end) {
				keywords.push(pos);
			}
		}

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
			string line;
			getline(is, line);

			// Stores the starting index of each keyword in assending order.
			// Which index referse to which keyword is determined later.
			priority_queue<size_t, vector<size_t>, greater<size_t>> keywords;

			// --- Find start of each keyword ---
			// --- Ignore keywords found after "string" --- 
			size_t stringBegin = line.find("string");

			findAndPushKeyword(keywords, line, "string", string::npos);
			findAndPushKeyword(keywords, line, "depth", stringBegin);
			findAndPushKeyword(keywords, line, "seldepth", stringBegin);
			findAndPushKeyword(keywords, line, "time", stringBegin);
			findAndPushKeyword(keywords, line, "nodes", stringBegin);
			findAndPushKeyword(keywords, line, "pv", stringBegin);
			findAndPushKeyword(keywords, line, "multipv", stringBegin);
			findAndPushKeyword(keywords, line, "score", stringBegin);
			findAndPushKeyword(keywords, line, "currmove", stringBegin);
			findAndPushKeyword(keywords, line, "currmovenumber", stringBegin);
			findAndPushKeyword(keywords, line, "hashfull", stringBegin);
			findAndPushKeyword(keywords, line, "nps", stringBegin);
			findAndPushKeyword(keywords, line, "tbhits", stringBegin);
			findAndPushKeyword(keywords, line, "sbhits", stringBegin);
			findAndPushKeyword(keywords, line, "cpuload", stringBegin);
			findAndPushKeyword(keywords, line, "refutation", stringBegin);
			findAndPushKeyword(keywords, line, "currline", stringBegin);
			keywords.push(line.size());	// Push the end so the last token isn't ignored

			// --- Split each section by keywords ---
			queue<stringstream> parts;

			if (keywords.size()) {
				size_t start = keywords.top();
				keywords.pop();

				while (keywords.size()) {
					size_t end = keywords.top();
					keywords.pop();

					string keyvalue(line, start, end - start);

					parts.emplace(keyvalue);

					start = end;
				}
			}

			// --- Tokenize each --- 

			while (parts.size()) {
				// --- extract info key ---
				stringstream ss = move(parts.front());
				parts.pop();

				string token;
				ss >> token;

				// --- extract info value (based on the key) ---
				if (token == "depth")				ss >> info.depth;
				else if (token == "seldepth")		ss >> info.seldepth;
				else if (token == "time")			ss >> info.time;
				else if (token == "nodes")			ss >> info.nodes;
				else if (token == "pv")				ss >> info.pv;
				else if (token == "multipv")		ss >> info.multipv;
				else if (token == "score")			ss >> info.score;
				else if (token == "currmove")		ss >> info.currmove;
				else if (token == "currmovenumber")	ss >> info.currmovenumber;
				else if (token == "hashfull")		ss >> info.hashfull;
				else if (token == "nps")			ss >> info.nps;
				else if (token == "tbhits")			ss >> info.tbhits;
				else if (token == "sbhits")			ss >> info.sbhits;
				else if (token == "cpuload")		ss >> info.cpuload;
				else if (token == "refutation")		ss >> info.refutation;
				else if (token == "currline")		ss >> info.currline;
				else if (token == "string")			info.string = ss.str();
				else {
					cout << "Uhho: " << token << ss.str() << endl;
				}
			}

			return is;
		}
	} // namespace uci
} // namespace forge