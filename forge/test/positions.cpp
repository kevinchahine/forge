#include "positions.h"

using namespace std;

namespace forge
{
	namespace test
	{
		vector<Position> g_positions;

		void init()
		{
			g_positions.clear();
			g_positions.reserve(20);

			Position pos;
			pos.setupNewGame();
			g_positions.emplace_back(pos);

			//g_positions.emplace_back("rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w - - 1 2");
			//g_positions.emplace_back("r2qkb1r/ppp1pBpp/2np1nb1/8/4PP2/P7/1PPP2PP/RNBQ1KNR b - - 0 1");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
			//g_positions.emplace_back("rnb1kbnr/2qP1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnb1kbnr/3Pqppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("1nbqkbnr/3P1ppp/r3p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rn1qkbnr/1b1P1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbqkbnr/3P1pp1/4p3/p6p/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnb1kbnr/3P1ppp/1q2p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbqkbnr/3P1p1p/4p3/p5p1/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/1NBQKB3 b - - 0 5");
			//g_positions.emplace_back("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/1NBQKB3 w - - 0 5");
			//g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/8/PPPN2PP/R1BQKBNR w KQkq - 0 6");
			//g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR b KQkq - 0 6");
			//g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR w KQkq - 1 7");
			//g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P2N2/PP4PP/R1BQKBNR b KQkq - 2 7");
			//g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3p1P2/2P2N2/PP4PP/R1BQKBNR w KQkq - 0 8");
			//g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR b KQkq - 0 8");
			//g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR w KQkq - 0 9");
			//g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/3B1N2/PP4PP/R1BQK1NR b KQkq - 1 9");
			//g_positions.emplace_back("r1bqk2r/pp1n2pp/2n1pp2/3pP3/1b1P1P2/3B1N2/PP4PP/R1BQK1NR w KQkq - 2 10");
			//g_positions.emplace_back("4r1k1/2qb1pp1/5n1p/pp6/2Pp4/3P3P/1PBN1PP1/R2Q2K1 b - - 1 26");
			//g_positions.emplace_back("1r4k1/2qb1pp1/5n1p/pp6/2Pp4/3P3P/1PBN1PP1/R2Q2K1 w - - 2 27");
			//g_positions.emplace_back("1r4k1/2qb1pp1/5n1p/pP6/3p4/3P3P/1PBN1PP1/R2Q2K1 b - - 0 27");
			//g_positions.emplace_back("6k1/2qb1pp1/5n1p/pr6/3p4/3P3P/1PBN1PP1/R2Q2K1 w - - 0 28");
			//g_positions.emplace_back("6k1/2qb1pp1/5n1p/pr6/B2p4/3P3P/1P1N1PP1/R2Q2K1 b - - 1 28");
			//g_positions.emplace_back("6k1/2qb1pp1/5n1p/p7/B2p4/3P3P/1r1N1PP1/R2Q2K1 w - - 0 29");
			//g_positions.emplace_back("6k1/2qB1pp1/5n1p/p7/3p4/3P3P/1r1N1PP1/R2Q2K1 b - - 0 29");
			//g_positions.emplace_back("6k1/2qn1pp1/7p/p7/3p4/3P3P/1r1N1PP1/R2Q2K1 w - - 0 30");
			//g_positions.emplace_back("6k1/2qn1pp1/7p/p7/2Np4/3P3P/1r3PP1/R2Q2K1 b - - 1 30");
			//g_positions.emplace_back("6k1/2qn1pp1/7p/pr6/2Np4/3P3P/5PP1/R2Q2K1 w - - 2 31");

			g_positions.emplace_back("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
			g_positions.emplace_back("rnbqkbnr/pppp1ppp/4p3/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
			g_positions.emplace_back("rnbqkbnr/pppp1ppp/4p3/8/3PP3/8/PPP2PPP/RNBQKBNR b KQkq - 0 2");
			g_positions.emplace_back("rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3");
			g_positions.emplace_back("rnbqkbnr/ppp2ppp/4p3/3p4/3PP3/8/PPPN1PPP/R1BQKBNR b KQkq - 1 3");
			g_positions.emplace_back("rnbqkb1r/ppp2ppp/4pn2/3p4/3PP3/8/PPPN1PPP/R1BQKBNR w KQkq - 2 4");
			g_positions.emplace_back("rnbqkb1r/ppp2ppp/4pn2/3pP3/3P4/8/PPPN1PPP/R1BQKBNR b KQkq - 0 4");
			g_positions.emplace_back("rnbqkb1r/pppn1ppp/4p3/3pP3/3P4/8/PPPN1PPP/R1BQKBNR w KQkq - 1 5");
			g_positions.emplace_back("rnbqkb1r/pppn1ppp/4p3/3pP3/3P1P2/8/PPPN2PP/R1BQKBNR b KQkq - 0 5");
			g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/8/PPPN2PP/R1BQKBNR w KQkq - 0 6");
			g_positions.emplace_back("rnbqkb1r/pp1n1ppp/4p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR b KQkq - 0 6");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P5/PP1N2PP/R1BQKBNR w KQkq - 1 7");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/2ppP3/3P1P2/2P2N2/PP4PP/R1BQKBNR b KQkq - 2 7");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3p1P2/2P2N2/PP4PP/R1BQKBNR w KQkq - 0 8");
			g_positions.emplace_back("r1bqkb1r/pp1n1ppp/2n1p3/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR b KQkq - 0 8");
			g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/5N2/PP4PP/R1BQKBNR w KQkq - 0 9");
			g_positions.emplace_back("r1bqkb1r/pp1n2pp/2n1pp2/3pP3/3P1P2/3B1N2/PP4PP/R1BQK1NR b KQkq - 1 9");
			g_positions.emplace_back("r1bqk2r/pp1n2pp/2n1pp2/3pP3/1b1P1P2/3B1N2/PP4PP/R1BQK1NR w KQkq - 2 10");
			g_positions.emplace_back("r1bqk2r/pp1n2pp/2n1pp2/3pP3/1b1P1P2/3B1N2/PP1B2PP/R2QK1NR b KQkq - 3 10");
			g_positions.emplace_back("r1b1k2r/pp1n2pp/1qn1pp2/3pP3/1b1P1P2/3B1N2/PP1B2PP/R2QK1NR w KQkq - 4 11");
			g_positions.emplace_back("r1b1k2r/pp1n2pp/1qn1pp2/3pP3/1b1P1P2/3B1N2/PP1BN1PP/R2QK2R b KQkq - 5 11");
			g_positions.emplace_back("r1b1k2r/pp1n2pp/1qn1p3/3pp3/1b1P1P2/3B1N2/PP1BN1PP/R2QK2R w KQkq - 0 12");
			g_positions.emplace_back("r1b1k2r/pp1n2pp/1qn1p3/3pP3/1b1P4/3B1N2/PP1BN1PP/R2QK2R b KQkq - 0 12");
			g_positions.emplace_back("r1b2rk1/pp1n2pp/1qn1p3/3pP3/1b1P4/3B1N2/PP1BN1PP/R2QK2R w KQ - 1 13");
			g_positions.emplace_back("r1b2rk1/pp1n2pp/1qn1p3/3pP3/1b1P4/P2B1N2/1P1BN1PP/R2QK2R b KQ - 0 13");
			g_positions.emplace_back("r1b2rk1/pp1nb1pp/1qn1p3/3pP3/3P4/P2B1N2/1P1BN1PP/R2QK2R w KQ - 1 14");
			g_positions.emplace_back("r1b2rk1/pp1nb1pp/1qn1p3/3pP3/3P4/P2B1N2/1PQBN1PP/R3K2R b KQ - 2 14");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/1qn1p3/3pP3/3P4/P2B1r2/1PQBN1PP/R3K2R w KQ - 0 15");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/1qn1p3/3pP3/3P4/P2B1P2/1PQBN2P/R3K2R b KQ - 0 15");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/1q2p3/3pP3/3n4/P2B1P2/1PQBN2P/R3K2R w KQ - 0 16");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/1q2p3/3pP3/3N4/P2B1P2/1PQB3P/R3K2R b KQ - 0 16");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/4p3/3pP3/3q4/P2B1P2/1PQB3P/R3K2R w KQ - 0 17");
			g_positions.emplace_back("r1b3k1/pp1nb1pp/4p3/3pP3/3q4/P2B1P2/1PQB3P/2KR3R b - - 1 17");
			g_positions.emplace_back("r1b3k1/pp2b1pp/4p3/3pn3/3q4/P2B1P2/1PQB3P/2KR3R w - - 0 18");
			g_positions.emplace_back("r1b3k1/pp2b1pB/4p3/3pn3/3q4/P4P2/1PQB3P/2KR3R b - - 0 18");
			g_positions.emplace_back("r1b4k/pp2b1pB/4p3/3pn3/3q4/P4P2/1PQB3P/2KR3R w - - 1 19");
			g_positions.emplace_back("r1b4k/pp2b1pB/4p3/3pn3/3q4/P4P2/1PQB3P/1K1R3R b - - 2 19");
			g_positions.emplace_back("r1b4k/pp2b1pB/4p3/3pn3/7q/P4P2/1PQB3P/1K1R3R w - - 3 20");
			g_positions.emplace_back("r1b4k/pp2b1pB/4p3/3pn3/7q/P1B2P2/1PQ4P/1K1R3R b - - 4 20");
			g_positions.emplace_back("r1b4k/pp4pB/4pb2/3pn3/7q/P1B2P2/1PQ4P/1K1R3R w - - 5 21");
			g_positions.emplace_back("r1b4k/pp4pB/4pb2/3pn3/5P1q/P1B5/1PQ4P/1K1R3R b - - 0 21");
			g_positions.emplace_back("r1b4k/pp4pB/4pb2/3p4/2n2P1q/P1B5/1PQ4P/1K1R3R w - - 1 22");
			g_positions.emplace_back("r1b4k/pp4pB/4pB2/3p4/2n2P1q/P7/1PQ4P/1K1R3R b - - 0 22");
			g_positions.emplace_back("r1b4k/pp4pB/4pq2/3p4/2n2P2/P7/1PQ4P/1K1R3R w - - 0 23");
			g_positions.emplace_back("r1b4k/pp4p1/4pq2/3p4/2n2P2/P2B4/1PQ4P/1K1R3R b - - 1 23");
			g_positions.emplace_back("r1b4k/p5p1/4pq2/1p1p4/2n2P2/P2B4/1PQ4P/1K1R3R w - - 0 24");
			g_positions.emplace_back("r1b4k/p5p1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K1R3R b - - 1 24");
			g_positions.emplace_back("r6k/p2b2p1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K1R3R w - - 2 25");
			g_positions.emplace_back("r6k/p2b2p1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K1R2R1 b - - 3 25");
			g_positions.emplace_back("r3b2k/p5p1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K1R2R1 w - - 4 26");
			g_positions.emplace_back("r3b2k/p5p1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K2R1R1 b - - 5 26");
			g_positions.emplace_back("r6k/p4bp1/4pq2/1p1p4/2n2P2/P2B4/1P2Q2P/1K2R1R1 w - - 6 27");
			g_positions.emplace_back("r6k/p4bp1/4pq2/1p1p4/2n2P2/P2B2R1/1P2Q2P/1K2R3 b - - 7 27");
			g_positions.emplace_back("2r4k/p4bp1/4pq2/1p1p4/2n2P2/P2B2R1/1P2Q2P/1K2R3 w - - 8 28");
			g_positions.emplace_back("2r4k/p4bp1/4pq2/1p1p4/2n2P2/P2B2R1/1P2Q2P/1K4R1 b - - 9 28");
			g_positions.emplace_back("2r4k/p4bp1/3npq2/1p1p4/5P2/P2B2R1/1P2Q2P/1K4R1 w - - 10 29");
			g_positions.emplace_back("2r4k/p4bR1/3npq2/1p1p4/5P2/P2B4/1P2Q2P/1K4R1 b - - 0 29");
			g_positions.emplace_back("2r4k/p4bR1/4pq2/1p1p1n2/5P2/P2B4/1P2Q2P/1K4R1 w - - 1 30");
			g_positions.emplace_back("2r4k/p4b2/4pq2/1p1p1nR1/5P2/P2B4/1P2Q2P/1K4R1 b - - 2 30");
			g_positions.emplace_back("7k/p1r2b2/4pq2/1p1p1nR1/5P2/P2B4/1P2Q2P/1K4R1 w - - 3 31");
			g_positions.emplace_back("7k/p1r2b2/4pq2/1p1p1BR1/5P2/P7/1P2Q2P/1K4R1 b - - 0 31");
			g_positions.emplace_back("7k/p1r2b2/5q2/1p1p1pR1/5P2/P7/1P2Q2P/1K4R1 w - - 0 32");
			g_positions.emplace_back("7k/p1r2b2/5q2/1p1p1p1R/5P2/P7/1P2Q2P/1K4R1 b - - 1 32");
			g_positions.emplace_back("rnbqkbnr/pp3ppp/4p3/2pp4/3PP3/8/PPPN1PPP/R1BQKBNR w KQkq - 0 4");
			g_positions.emplace_back("rnbqkbnr/pp3ppp/4p3/2pP4/3P4/8/PPPN1PPP/R1BQKBNR b KQkq - 0 4");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/4p3/2pq4/3P4/8/PPPN1PPP/R1BQKBNR w KQkq - 0 5");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/4p3/2pq4/3P4/5N2/PPPN1PPP/R1BQKB1R b KQkq - 1 5");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/4p3/3q4/3p4/5N2/PPPN1PPP/R1BQKB1R w KQkq - 0 6");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/4p3/3q4/2Bp4/5N2/PPPN1PPP/R1BQK2R b KQkq - 1 6");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/3qp3/8/2Bp4/5N2/PPPN1PPP/R1BQK2R w KQkq - 2 7");
			g_positions.emplace_back("rnb1kbnr/pp3ppp/3qp3/8/2Bp4/5N2/PPPN1PPP/R1BQ1RK1 b kq - 3 7");
			g_positions.emplace_back("rnb1kb1r/pp3ppp/3qpn2/8/2Bp4/5N2/PPPN1PPP/R1BQ1RK1 w kq - 4 8");
			g_positions.emplace_back("rnb1kb1r/pp3ppp/3qpn2/8/2Bp4/1N3N2/PPP2PPP/R1BQ1RK1 b kq - 5 8");
			g_positions.emplace_back("r1b1kb1r/pp3ppp/2nqpn2/8/2Bp4/1N3N2/PPP2PPP/R1BQ1RK1 w kq - 6 9");
			g_positions.emplace_back("r1b1kb1r/pp3ppp/2nqpn2/8/2BN4/5N2/PPP2PPP/R1BQ1RK1 b kq - 0 9");
			g_positions.emplace_back("r1b1kb1r/pp3ppp/3qpn2/8/2Bn4/5N2/PPP2PPP/R1BQ1RK1 w kq - 0 10");
			g_positions.emplace_back("r1b1kb1r/pp3ppp/3qpn2/8/2BN4/8/PPP2PPP/R1BQ1RK1 b kq - 0 10");
			g_positions.emplace_back("r1b1kb1r/1p3ppp/p2qpn2/8/2BN4/8/PPP2PPP/R1BQ1RK1 w kq - 0 11");
			g_positions.emplace_back("r1b1kb1r/1p3ppp/p2qpn2/8/2B5/5N2/PPP2PPP/R1BQ1RK1 b kq - 1 11");
			g_positions.emplace_back("r1b1kb1r/5ppp/p2qpn2/1p6/2B5/5N2/PPP2PPP/R1BQ1RK1 w kq - 0 12");
			g_positions.emplace_back("r1b1kb1r/5ppp/p2qpn2/1p6/8/3B1N2/PPP2PPP/R1BQ1RK1 b kq - 1 12");
			g_positions.emplace_back("r3kb1r/1b3ppp/p2qpn2/1p6/8/3B1N2/PPP2PPP/R1BQ1RK1 w kq - 2 13");
			g_positions.emplace_back("r3kb1r/1b3ppp/p2qpn2/1p6/P7/3B1N2/1PP2PPP/R1BQ1RK1 b kq - 0 13");
			g_positions.emplace_back("r3kb1r/1b3ppp/p2qp3/1p6/P5n1/3B1N2/1PP2PPP/R1BQ1RK1 w kq - 1 14");
			g_positions.emplace_back("r3kb1r/1b3ppp/p2qp3/1p6/P5n1/3B1N2/1PP2PPP/R1BQR1K1 b kq - 2 14");
			g_positions.emplace_back("r3kb1r/1b3ppp/pq2p3/1p6/P5n1/3B1N2/1PP2PPP/R1BQR1K1 w kq - 3 15");
			g_positions.emplace_back("r3kb1r/1b3ppp/pq2p3/1p6/P5n1/3B1N2/1PP1QPPP/R1B1R1K1 b kq - 4 15");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/1pb5/P5n1/3B1N2/1PP1QPPP/R1B1R1K1 w kq - 5 16");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/1pb5/P5n1/3B1N2/1PP1QPPP/R1B2RK1 b kq - 6 16");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/2b5/Pp4n1/3B1N2/1PP1QPPP/R1B2RK1 w kq - 0 17");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/2b5/Pp4n1/3B1N1P/1PP1QPP1/R1B2RK1 b kq - 0 17");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2pn2/2b5/Pp6/3B1N1P/1PP1QPP1/R1B2RK1 w kq - 1 18");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2pn2/2b3B1/Pp6/3B1N1P/1PP1QPP1/R4RK1 b kq - 2 18");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/2b3Bn/Pp6/3B1N1P/1PP1QPP1/R4RK1 w kq - 3 19");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/2b4n/Pp6/3BBN1P/1PP1QPP1/R4RK1 b kq - 4 19");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/7n/Pp6/3BbN1P/1PP1QPP1/R4RK1 w kq - 0 20");
			g_positions.emplace_back("r3k2r/1b3ppp/pq2p3/7n/Pp6/3BQN1P/1PP2PP1/R4RK1 b kq - 0 20");
			g_positions.emplace_back("r3k2r/1b3ppp/p3p3/7n/Pp6/3BqN1P/1PP2PP1/R4RK1 w kq - 0 21");
			g_positions.emplace_back("r3k2r/1b3ppp/p3p3/7n/Pp6/3BPN1P/1PP3P1/R4RK1 b kq - 0 21");
		}
	} // namespace test
} // namespace forge