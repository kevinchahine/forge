#include "TestFeatureExtractor.h"

#include "FeatureExtractor.h"

#include <iostream>

using namespace std;

namespace forge
{
	namespace test
	{
		namespace feature_extractor
		{
			void init()
			{
				Position position(
					//"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"
					"r1R1B1r1/8/1R1r1B1R/8/r1R1b1r1/8/1R1r1b1R/k1K5 w - - 0 1"
				);

				cout << "Its " << (position.isWhitesTurn() ? "whites" : "blacks") << " turn" << endl;

				position.board().printMini();

				FeatureExtractor extractor;

				extractor.init(position);
			}

			void findAllAttacked()
			{
				vector<Position> positions;
				
				positions.emplace_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
				positions.emplace_back("k7/8/r1R1r1R1/8/8/8/8/K7 b - - 0 1");
				positions.emplace_back("r1R1B1r1/8/1R1r1B1R/8/r1R1b1r1/8/1R1r1b1R/K1k5 w - - 0 1");
				positions.emplace_back("q1Q1B1q1/8/1Q1r1B1Q/8/q1Q1b1q1/8/1Q1q1b1Q/k1K5 w - - 0 1");

				FeatureExtractor extractor;

				for (const Position& pos : positions) {
					extractor.init(pos);

					BitBoard attacked = extractor.findAllAttacked();

					Board board = (pos.isWhitesTurn() ? pos.board() : pos.board().rotated());

					guten::core::Matrix boardMat = board.getMiniBoard();
					guten::core::Matrix attackedMat = attacked.toMat();

					guten::core::Matrix mat{
						max(boardMat.nRows(), attackedMat.nRows()),		// nrows
						boardMat.nCols() + attackedMat.nCols(),			// ncols
					};

					boardMat.copyTo(mat, guten::Point{ 0, 0 });
					attackedMat.copyTo(mat, guten::Point{ 0, boardMat.nCols() });

					cout << "Its " << (pos.isWhitesTurn() ? "whites" : "blacks") << " turn" << endl;
					cout << "In perspective of moving player" << endl;
					mat.print(1);

					cout << endl;
				}
			}

			void countAllAttacked()
			{
				vector<Position> positions;

				positions.emplace_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
				positions.emplace_back("k7/8/r1R1r1R1/8/8/8/8/K7 b - - 0 1");
				positions.emplace_back("r1R1B1r1/8/1R1r1B1R/8/r1R1b1r1/8/1R1r1b1R/K1k5 w - - 0 1");
				positions.emplace_back("q1Q1B1q1/8/1Q1r1B1Q/8/q1Q1b1q1/8/1Q1q1b1Q/k1K5 w - - 0 1");

				FeatureExtractor extractor;

				for (const Position& pos : positions) {
					extractor.init(pos);

					IntBoard attackedCounts = extractor.countAllAttacked();

					Board board = (pos.isWhitesTurn() ? pos.board() : pos.board().rotated());

					guten::core::Matrix boardMat = board.getMiniBoard();
					//guten::core::Matrix attackedMat = attackedCounts.toMat();
					
					//guten::core::Matrix mat{
					//	max(boardMat.nRows(), attackedMat.nRows()),		// nrows
					//	boardMat.nCols() + attackedMat.nCols(),			// ncols
					//};

					//boardMat.copyTo(mat, guten::Point{ 0, 0 });
					//attackedMat.copyTo(mat, guten::Point{ 0, boardMat.nCols() });

					cout << "Its " << (pos.isWhitesTurn() ? "whites" : "blacks") << " turn" << endl;
					cout << "In perspective of moving player" << endl;
					
					boardMat.print();
					cout << guten::color::white;
					attackedCounts.print();

					//mat.print(1);

					cout << endl;
				}
			}
		} // namespace feature_extractor
	} // namespace test
} // namespace forge