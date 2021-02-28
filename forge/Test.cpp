#pragma once

#include "Test.h"

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
			
			clock.start();
			
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

		void board()
		{
			forge::Board b;

			b.setPiece(0, 0, pieces::blackRook);
			b.setPiece(0, 1, pieces::blackKnight);
			b.setPiece(0, 2, pieces::blackBishop);
			b.setPiece(0, 3, pieces::blackQueen);
			b.setPiece(0, 4, pieces::blackKing);
			b.setPiece(0, 5, pieces::blackBishop);
			b.setPiece(0, 6, pieces::blackKnight);
			b.setPiece(0, 7, pieces::blackRook);
			
			b.setPiece(1, 0, pieces::blackPawn);
			b.setPiece(1, 1, pieces::blackPawn);
			b.setPiece(1, 2, pieces::blackPawn);
			b.setPiece(1, 3, pieces::blackPawn);
			b.setPiece(1, 4, pieces::blackPawn);
			b.setPiece(1, 5, pieces::blackPawn);
			b.setPiece(1, 6, pieces::blackPawn);
			b.setPiece(1, 7, pieces::blackPawn);
			
			b.setPiece(6, 0, pieces::whitePawn);
			b.setPiece(6, 1, pieces::whitePawn);
			b.setPiece(6, 2, pieces::whitePawn);
			b.setPiece(6, 3, pieces::whitePawn);
			b.setPiece(6, 4, pieces::whitePawn);
			b.setPiece(6, 5, pieces::whitePawn);
			b.setPiece(6, 6, pieces::whitePawn);
			b.setPiece(6, 7, pieces::whitePawn);
			
			b.setPiece(7, 0, pieces::whiteRook);
			b.setPiece(7, 1, pieces::whiteKnight);
			b.setPiece(7, 2, pieces::whiteBishop);
			b.setPiece(7, 3, pieces::whiteQueen);
			b.setPiece(7, 4, pieces::whiteKing);
			b.setPiece(7, 5, pieces::whiteBishop);
			b.setPiece(7, 6, pieces::whiteKnight);
			b.setPiece(7, 7, pieces::whiteRook);

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
	} // namespace test
} // namespace forge