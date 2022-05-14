#include "positions.h"

using namespace std;

namespace forge
{
	namespace test
	{
		vector<Position> g_positions;

		void init()
		{
			g_positions.reserve(20);

			Position pos;
			pos.reset();
			g_positions.emplace_back(pos);

			g_positions.emplace_back("rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w - - 1 2");
			g_positions.emplace_back("r2qkb1r/ppp1pBpp/2np1nb1/8/4PP2/P7/1PPP2PP/RNBQ1KNR b - - 0 1");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			g_positions.emplace_back("rnb1kbnr/2qP1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnb1kbnr/3Pqppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("1nbqkbnr/3P1ppp/r3p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rn1qkbnr/1b1P1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbqkbnr/3P1pp1/4p3/p6p/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnb1kbnr/3P1ppp/1q2p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbqkbnr/3P1p1p/4p3/p5p1/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/1NBQKB3 b - - 0 5");
			g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/1NBQKB3 w - - 0 5");

		}
	} // namespace test
} // namespace forge