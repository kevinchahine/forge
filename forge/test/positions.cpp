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
			pos.setupNewGame();
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
			g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/8/PPPN2PP/R1BQKBNR w KQkq - 0 6");
			g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR b KQkq - 0 6");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR w KQkq - 1 7");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P2N2/PP4PP/R1BQKBNR b KQkq - 2 7");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3p1P2/2P2N2/PP4PP/R1BQKBNR w KQkq - 0 8");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR b KQkq - 0 8");
			g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR w KQkq - 0 9");
			g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/3B1N2/PP4PP/R1BQK1NR b KQkq - 1 9");
			g_positions.emplace_back("r1bqk2r/pp1n2pp/2n1pp2/3pP3/1b1P1P2/3B1N2/PP4PP/R1BQK1NR w KQkq - 2 10");
			g_positions.emplace_back("4r1k1/2qb1pp1/5n1p/pp6/2Pp4/3P3P/1PBN1PP1/R2Q2K1 b - - 1 26");
			g_positions.emplace_back("1r4k1/2qb1pp1/5n1p/pp6/2Pp4/3P3P/1PBN1PP1/R2Q2K1 w - - 2 27");
			g_positions.emplace_back("1r4k1/2qb1pp1/5n1p/pP6/3p4/3P3P/1PBN1PP1/R2Q2K1 b - - 0 27");
			g_positions.emplace_back("6k1/2qb1pp1/5n1p/pr6/3p4/3P3P/1PBN1PP1/R2Q2K1 w - - 0 28");
			g_positions.emplace_back("6k1/2qb1pp1/5n1p/pr6/B2p4/3P3P/1P1N1PP1/R2Q2K1 b - - 1 28");
			g_positions.emplace_back("6k1/2qb1pp1/5n1p/p7/B2p4/3P3P/1r1N1PP1/R2Q2K1 w - - 0 29");
			g_positions.emplace_back("6k1/2qB1pp1/5n1p/p7/3p4/3P3P/1r1N1PP1/R2Q2K1 b - - 0 29");
			g_positions.emplace_back("6k1/2qn1pp1/7p/p7/3p4/3P3P/1r1N1PP1/R2Q2K1 w - - 0 30");
			g_positions.emplace_back("6k1/2qn1pp1/7p/p7/2Np4/3P3P/1r3PP1/R2Q2K1 b - - 1 30");
			g_positions.emplace_back("6k1/2qn1pp1/7p/pr6/2Np4/3P3P/5PP1/R2Q2K1 w - - 2 31");
		}
	} // namespace test
} // namespace forge