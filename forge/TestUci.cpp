#include "TestUci.h"

using namespace std;

namespace forge
{
	namespace uci
	{
		namespace test
		{
			void engineToGui()
			{
				stringstream cmd;
				cmd << "info depth 1 seldepth 1 multipv 1 score cp 29 nodes 20 nps 20000 tbhits 0 time 1 pv d2d4";

				cout << "This:             \'" << cmd.str() << '\'' << endl;

				UciInfo info;
				cmd >> info;

				cout << "Should look like: \'" << info << '\'' << endl;
			}

			void info()
			{
				UciInfo info;

				info.depth = 26;
				info.seldepth = 27;
				info.time = 1000;
				info.nodes = 100'000;
				info.pv = vector<Move>{ Move{"a4a5"}, Move{"a6a7"}, Move{"b4c4"} };
				info.multipv = 4;
				info.score = UciScore{};
				info.currmove = Move{ "e2e4" };
				info.currmovenumber = 23;
				info.hashfull = 24;
				info.nps = 222;
				info.tbhits = 333;
				info.sbhits = 444;
				info.cpuload = 99;
				info.string = "Just a message from the engine";
				info.refutation	= vector<Move>{ Move{"d4d5"}, Move{"f6f7"}, Move{"g4g4"} };
				info.currline = UciCurrLine{};

				stringstream ss;
				ss << info << endl;

				cout << "Here is the serialized UciInfo: " << endl << '\t' << info << endl;

				UciInfo info2;
				ss >> info2;

				cout << "Here is the parsed UciInfo: " << endl << '\t' << info2 << endl;
			}
		} // namespace test
	} // namespace uci
} // namespace forge