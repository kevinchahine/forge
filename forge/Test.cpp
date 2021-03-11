#pragma once

#include "Test.h"

#include "Guten/GridView.h"

using namespace std;

namespace forge
{
	namespace test
	{
		void clock()
		{
			forge::Clock clock;
			
			clock.synchronize(
				chrono::minutes(10),
				chrono::seconds(5),
				chrono::minutes(10),
				chrono::seconds(5)
			);
			
			clock.click();
			
			while (true)
			{
				if (_kbhit()) {
					char ch = _getch();
			
					clock.click();
				}
			
				cout << clock << '\n';
					
				this_thread::sleep_for(chrono::seconds(1));
			}
		}

		void boardDefualt()
		{
			forge::Board b;

			b.reset();

			b.print();
		}

		void board()
		{
			forge::Board b;

			b.placePiece(0, 0, pieces::blackRook);
			b.placePiece(0, 1, pieces::blackKnight);
			b.placePiece(0, 2, pieces::blackBishop);
			b.placePiece(0, 3, pieces::blackQueen);
			b.placePiece(0, 4, pieces::blackKing);
			b.placePiece(0, 5, pieces::blackBishop);
			b.placePiece(0, 6, pieces::blackKnight);
			b.placePiece(0, 7, pieces::blackRook);
			
			b.placePiece(1, 0, pieces::blackPawn);
			b.placePiece(1, 1, pieces::blackPawn);
			b.placePiece(1, 2, pieces::blackPawn);
			b.placePiece(1, 3, pieces::blackPawn);
			b.placePiece(1, 4, pieces::blackPawn);
			b.placePiece(1, 5, pieces::blackPawn);
			b.placePiece(1, 6, pieces::blackPawn);
			b.placePiece(1, 7, pieces::blackPawn);
			
			b.placePiece(6, 0, pieces::whitePawn);
			b.placePiece(6, 1, pieces::whitePawn);
			b.placePiece(6, 2, pieces::whitePawn);
			b.placePiece(6, 3, pieces::whitePawn);
			b.placePiece(6, 4, pieces::whitePawn);
			b.placePiece(6, 5, pieces::whitePawn);
			b.placePiece(6, 6, pieces::whitePawn);
			b.placePiece(6, 7, pieces::whitePawn);
			
			b.placePiece(7, 0, pieces::whiteRook);
			b.placePiece(7, 1, pieces::whiteKnight);
			b.placePiece(7, 2, pieces::whiteBishop);
			b.placePiece(7, 3, pieces::whiteQueen);
			b.placePiece(7, 4, pieces::whiteKing);
			b.placePiece(7, 5, pieces::whiteBishop);
			b.placePiece(7, 6, pieces::whiteKnight);
			b.placePiece(7, 7, pieces::whiteRook);

			cout << setw(10) << "pawns: " << b.pawns() << endl;
			cout << setw(10) << "rooks: " << b.rooks() << endl;
			cout << setw(10) << "bishops: " << b.bishops() << endl;
			cout << setw(10) << "queens: " << b.queens() << endl;
			cout << setw(10) << "knights: " << b.knights() << endl;
			cout << setw(10) << "kings: " << b.kings() << endl;
			cout << setw(10) << "whites: " << b.whites() << endl;
			cout << setw(10) << "blacks: " << b.blacks() << endl;

			b.print();
		}

		void placeMethods()
		{
			forge::Board b;

			using Square = forge::BoardSquare;

			b.placeBlackRook(Square{ 'a', '8' });
			b.placeBlackKnight(Square{ 'b', '8' });
			b.placeBlackBishop(Square{ 'c', '8' });
			b.placeBlackQueen(Square{ 'd', '8' });
			b.moveBlackKing(Square{ 'e', '8' });	// Kings already here
			b.placeBlackBishop(Square{ 'f', '8' });
			b.placeBlackKnight(Square{ 'g', '8' });
			b.placeBlackRook(Square{ 'h', '8' });

			b.placeBlackPawn(Square{ 'a', '7' });
			b.placeBlackPawn(Square{ 'b', '7' });
			b.placeBlackPawn(Square{ 'c', '7' });
			b.placeBlackPawn(Square{ 'd', '7' });
			b.placeBlackPawn(Square{ 'e', '7' });
			b.placeBlackPawn(Square{ 'f', '7' });
			b.placeBlackPawn(Square{ 'g', '7' });
			b.placeBlackPawn(Square{ 'h', '7' });

			b.placeWhitePawn(Square{ 'a', '2' });
			b.placeWhitePawn(Square{ 'b', '2' });
			b.placeWhitePawn(Square{ 'c', '2' });
			b.placeWhitePawn(Square{ 'd', '2' });
			b.placeWhitePawn(Square{ 'e', '2' });
			b.placeWhitePawn(Square{ 'f', '2' });
			b.placeWhitePawn(Square{ 'g', '2' });
			b.placeWhitePawn(Square{ 'h', '2' });

			b.placeWhiteRook(Square{ 'a', '1' });
			b.placeWhiteKnight(Square{ 'b', '1' });
			b.placeWhiteBishop(Square{ 'c', '1' });
			b.placeWhiteQueen(Square{ 'd', '1' });
			b.moveWhiteKing(Square{ 'e', '1' });	// Kings already here
			b.placeWhiteBishop(Square{ 'f', '1' });
			b.placeWhiteKnight(Square{ 'g', '1' });
			b.placeWhiteRook(Square{ 'h', '1' });

			b.print();
		}

		void psuedoValidMoveGenerator()
		{
			forge::Position p;
			forge::Board & b = p.board();

			///b.placeAllPieces();
			b.reset();										// remove
			forge::BoardSquare s{ 5, 4 };					// remove
			b.placeBlackRook(s);		// remove
			b.placeBlackRook(s.up(2));		// remove
			(const_cast<forge::MoveCounter &>(p.moveCounter()))++;
			
			b.print();

			forge::MoveList moves = 
				forge::MoveGenerator::generatePseudoMoves(p);

			cout << moves.size() << " legal moves generated" << endl;

			guten::grids::GridView gridView;
			gridView.setGridCols(8);

			for (int i = 0; i < moves.size(); i++) {
				const auto & elem = moves.at(i);

				guten::core::Matrix mini = elem.position.board().getMiniBoard();

				gridView.push(mini);

				cout << left << setw(9) << elem.move.toLAN();
			}
			cout << '\n';

			gridView.toMatrix().print();
		}

		void move()
		{
			forge::Move m;

			cout << "Empty constructor: " << m << endl;

			cout << forge::BoardSquare(1, 2) << endl;
			cout << forge::BoardSquare(2, 2) << endl;
			cout << forge::Move{ forge::BoardSquare(1, 2), forge::BoardSquare(2, 2) } << endl;

			cout << forge::BoardSquare(4, 5) << ' ' 
				<< forge::BoardSquare(6, 7) << ' '
				<< forge::pieces::whiteQueen << ' '
				<< forge::Move{ 
					forge::BoardSquare(4, 5), 
					forge::BoardSquare(6, 7), 
					forge::pieces::whiteQueen } 
			<< endl;

			cout << forge::BoardSquare('e', '2') << ' '
				<< forge::BoardSquare('e', '4') << ' '
				<< forge::Move(forge::BoardSquare('e', '2'), forge::BoardSquare('e', '4'))
				<< endl;
		}

		void randomSolver()
		{
			forge::RandomSolver solver;


		}

		void chessMatch()
		{
			forge::ChessMatch match;
			
			match.reset();

			match.position().clear();												// remove
			match.position().board().placeWhiteRook(forge::BoardSquare{ 4, 4 });	// remove
			match.position().board().placeBlackRook(forge::BoardSquare{ 5, 5 });	// remove

			match.clock().synchronize(
				chrono::minutes(5),
				chrono::minutes(5),
				chrono::seconds(2),
				chrono::seconds(2)
			); // Clock is still paused

			match.makeWhiteController<RandomSolver>();
			match.makeBlackController<RandomSolver>();

			match.runGame();
		}
	} // namespace test
} // namespace forge