#pragma once

#include "Test.h"
#include "TextView.h"

#include "Guten/GridView.h"

#include <sstream>

using namespace std;
using namespace forge::pieces;


namespace forge
{
	namespace test
	{
		// --- Global Variables ---
		vector<Position> positions;

		void initTestBoards()
		{
			using namespace pieces;
			using bs = BoardSquare;

			{
				Board & b = positions.emplace_back().board();
				b.placeAllPieces();
			}
			
			{
				Board & b = positions.emplace_back().board();
				b.place<pieces::Rook>(bs{ 'e', '2' }, BLACK);
				b.place<pieces::BlackPawn>(bs{ 'c', '2' });
				b.place<pieces::WhitePawn>(bs{ 'c', '5' });
				b.place<pieces::Bishop>(bs{ 'e', '7' }, WHITE);
				b.place<pieces::Queen>(bs{ 'e', '4' }, WHITE);
				b.place<pieces::Knight>(bs{ 'f', '8' }, WHITE);
			}
			
			{
				bs s{ 'e', '4' };
				Board & b = positions.emplace_back().board();
				b.place<pieces::Knight>(s, WHITE);
				b.place<pieces::Queen>(s.right(), WHITE);
				b.place<pieces::Bishop>(s.up(2), BLACK);
				b.place<pieces::WhitePawn>(s.left(2));
			}
			
			{
				bs k{ 'e', '4' };
				Board & b = positions.emplace_back().board();
				b.place<pieces::BlackKing>(k);
				b.place<pieces::Rook>(k.down(2), WHITE);
				b.place<pieces::Bishop>(k.downLeft(2), WHITE);
				b.place<pieces::Queen>(k.down(2), WHITE);
				b.place<pieces::Knight>(k.knight7(), WHITE);
				b.place<pieces::Pawn>(k.downLeft(), WHITE);
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs wk{ 'c', '3' };
				b.place<WhiteKing>(wk);
				//b.place<Knight>(wk.knight0(), BLACK);
				//b.place<Knight>(wk.knight1(), BLACK);
				b.place<Knight>(wk.knight2(), BLACK);
				b.place<Knight>(wk.knight3(), BLACK);
				b.place<Knight>(wk.knight4(), BLACK);
				b.place<Knight>(wk.knight5(), BLACK);
				b.place<Knight>(wk.knight5(), BLACK);
				b.place<Knight>(wk.knight6(), BLACK);
				b.place<Knight>(wk.knight7(), BLACK);
			
				bs bk{ 'f', '5' };
				b.place<BlackKing>(bk);
				//b.place<Knight>(bk.knight0(), WHITE);
				//b.place<Knight>(bk.knight1(), WHITE);
				b.place<Knight>(bk.knight2(), WHITE);
				b.place<Knight>(bk.knight3(), WHITE);
				b.place<Knight>(bk.knight4(), WHITE);
				b.place<Knight>(bk.knight5(), WHITE);
				b.place<Knight>(bk.knight5(), WHITE);
				b.place<Knight>(bk.knight6(), WHITE);
				b.place<Knight>(bk.knight7(), WHITE);
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs wk{ 'e', '3' };
				b.place<WhiteKing>(wk);
				b.place<Bishop>(wk.upLeft(2), BLACK);
				b.place<Bishop>(wk.upRight(2), WHITE);
				b.place<Queen>(wk.downLeft(2), WHITE);
				b.place<Queen>(wk.downRight(2), BLACK);
			
				bs bk{ 'e', '7' };
				b.place<BlackKing>(bk);
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs wk{ 'e', '3' };
				b.place<WhiteKing>(wk);
				b.place<Rook>(wk.up(4), BLACK);
			
				bs bk{ 'c', '5' };
				b.place<BlackKing>(bk);
				b.place<Rook>(bk.right(4), WHITE);
			
				Board & b2 = positions.emplace_back(positions.back()).board();
				b2.place<Bishop>(wk.upRight(3), BLACK);
				b2.place<Bishop>(bk.upRight(2), WHITE);
			
				Board & b3 = positions.emplace_back(positions.back()).board();
				b3.place<Queen>(bk.upLeft(2), WHITE);
				b3.place<Queen>(wk.downRight(2), BLACK);
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs wk{ 'f', '4' };
				b.place<WhiteKing>(wk);
				b.place<BlackPawn>(wk.upLeftOne());
				b.place<BlackPawn>(wk.upRightOne());
			
				bs bk{ 'c', '5' };
				b.place<BlackKing>(bk);
				b.place<WhitePawn>(bk.downLeftOne());
				b.place<WhitePawn>(bk.downRightOne());
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs wk{ 'e', '3' };
				b.place<Queen>(wk.up(3), BLACK);
				b.place<BlackPawn>(wk.upLeftOne());
				b.place<WhitePawn>(wk.downLeftOne());
			
			
				bs bk{ 'c', '6' };
				b.place<Queen>(bk.down(3), WHITE);
				b.place<WhitePawn>(bk.downRightOne());
				b.place<BlackPawn>(bk.upLeftOne());
			}
			
			{
				Board & b = positions.emplace_back().board();
			
				bs s{ 5, 4 };
				b.place<pieces::Knight>(s, WHITE);
				b.place<pieces::Queen>(s.right(), WHITE);
				b.place<pieces::Bishop>(s.up(2), BLACK);
				b.place<pieces::WhitePawn>(s.left(2));
			}
			
			{ // Pins
				Position & p = positions.emplace_back();
				const_cast<MoveCounter &>(p.moveCounter())++;	// Make it Blacks turn
				Board & b = p.board();
			
				bs bk{ 'e', '8' };
				b.place<BlackKing>(bk);
				b.place<WhiteKing>(bs{ 'c', '1' });
				b.place<Rook>(bk.down(2), BLACK);
				b.place<Queen>(bk.down(5), WHITE);
			} 
			
			{ // Pins
				Position & p = positions.emplace_back();
				const_cast<MoveCounter &>(p.moveCounter())++;	// Make it Blacks turn
				Board & b = p.board();
			
				bs bk{ 'e', '8' };
				b.place<BlackKing>(bk);
				b.place<WhiteKing>(bs{ 'c', '1' });
				b.place<Knight>(bk.down(2), BLACK);
				b.place<Queen>(bk.down(5), WHITE);
			}
			
			{ // Pins
				Position & p = positions.emplace_back();
				const_cast<MoveCounter &>(p.moveCounter())++;	// Make it Blacks turn
				Board & b = p.board();
			
				bs bk{ 'a', '4' };
				b.place<BlackKing>(bk);
				b.place<WhiteKing>(bs{ 'd', '1' });
				b.place<BlackPawn>(bk.right(3));
				b.place<Queen>(bk.right(7), WHITE);
			}
			
			{ // Pins
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'a', '8' });
				b.place<WhitePawn>(bs{ 'f', '7' });
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
				b.place<Bishop>(bs{ 'e', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'a', '8' });
				b.place<WhitePawn>(bs{ 'h', '7' });
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'a', '8' });
				b.place<WhitePawn>(bs{ 'f', '6' });
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'a', '8' });
				b.place<Knight>(bs{ 'f', '6' }, WHITE);
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'b', '8' });
				b.place<Rook>(bs{ 'f', '4' }, WHITE);
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'b', '8' });
				b.place<Bishop>(bs{ 'b', '7' }, WHITE);
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}
			
			{ // Block and Capture Attacker
				Position & p = positions.emplace_back();
				Board & b = p.board();
			
				b.place<WhiteKing>(bs{ 'b', '3' });
				b.place<BlackKing>(bs{ 'b', '8' });
				b.place<Rook>(bs{ 'f', '4' }, WHITE);
				b.place<Bishop>(bs{ 'b', '7' }, WHITE);
				b.place<Queen>(bs{ 'g', '8' }, BLACK);
			}

			{ // Promotions
				Position& p = positions.emplace_back();
				Board& b = p.board();

				b.place<WhiteKing>(bs{ 'd', '1' });
				b.place<BlackKing>(bs{ 'h', '6' });
				b.place<WhitePawn>(bs{ 'b', '7' });
			}

			{
				Position& p = positions.emplace_back();
				Board& b = p.board();

				p.move<WhiteKing>(Move{ b.whiteKing(), bs{ 'e', '1' } });
				b.place<BlackKing>(bs{ 'e', '7' });
				b.place<Rook>(bs{ 'b', '8' }, BLACK);
				b.place<BlackPawn>(bs{ 'b', '7' }, BLACK);
				b.place<Queen>(bs{ 'b', '4' }, WHITE);
			}

			{
				Position& p = positions.emplace_back();
				p.reset();
				Board& b = p.board();

				p.move<WhitePawn>(Move{ "f2f3" });
				p.move<BlackPawn>(Move{ "e7e6" });
				p.move<WhitePawn>(Move{ "g2g4" });
				p.move<Queen>(Move{ "d8h4" });
			}

			{
				Position& p = positions.emplace_back();
				p.reset();
				Board& b = p.board();

				p.move<WhitePawn>(Move{ "f2f3" });
				p.move<BlackPawn>(Move{ "e7e6" });
				p.move<WhitePawn>(Move{ "g2g4" });
				p.move<BlackPawn>(Move{ "a7a6" });
				p.move<Bishop>(Move{ "c1f4" });
				p.move<Queen>(Move{ "d8h4" });
			}

			{
				Position& p = positions.emplace_back();
				p.fromFEN(R"dil(rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR w - - 1 2)dil");
				p.move<BlackKing>(Move{ "e8e7" });
			}

			{
				Position& p = positions.emplace_back();
				
				Board& b = p.board();

				b.place<BlackKing>(bs{ 'd', '5' });
				b.place<WhiteKing>(bs{ 'a', '1' });
				b.place<Queen>(bs{ 'f', '5' }, WHITE);
				b.place<Queen>(bs{ 'h', '5' }, BLACK);
				
				p.move<WhiteKing>(Move{ "a1a2" });	// Now its Blacks Turn
			}

			{
				Position& p = positions.emplace_back();
				Board& b = p.board();

				b.place<BlackKing>(bs{ 'e', '7' });
				b.place<WhiteKing>(bs{ 'a', '1' });
				b.place<BlackPawn>(bs{ 'g', '7' });
				b.place<WhitePawn>(bs{ 'f', '6' });

				p.move<WhiteKing>(Move{ "a1a2" });
			}

			{
				Position& p = positions.emplace_back();
				p.fromFEN("r2qkb1r/ppp1pBpp/2np1nb1/8/4PP2/P7/1PPP2PP/RNBQ1KNR b - - 0 1");
			}

			{
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/RPP1PPPP/1NBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pp6/P7/5N2/1PPBPPPP/RN1QKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnb1kbnr/2qP1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnb1kbnr/3Pqppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("1nbqkbnr/3P1ppp/r3p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rn1qkbnr/1b1P1ppp/4p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbqkbnr/3P1pp1/4p3/p6p/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnb1kbnr/3P1ppp/1q2p3/p7/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbqkbnr/3P1p1p/4p3/p5p1/3P4/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
				positions.emplace_back().fromFEN("rnbq1bnr/2ppk1pp/3Ppp2/pP6/8/5N2/1PP1PPPP/RNBQKB1R b - - 0 5");
			}
		}

		void boardSquare()
		{
			BoardSquare cell;

			for (int r = 0; r < 8; r++) {
				for (int c = 0; c < 8; c++) {
					cell.row(r);
					cell.col(c);

					cout << cell << " is a " << (cell.isLightSquare() ? "light" : "dark")
						<< " square\n";
				}
			}
		}

		namespace bitboard {
			void allShifts()
			{
				BitBoard bb;

				bb[BoardSquare{ 'e', '4' }] = 1;
				bb[BoardSquare{ 'e', '5' }] = 1;
				bb[BoardSquare{ 'e', '6' }] = 1;
				bb[BoardSquare{ 'd', '4' }] = 1;
				bb[BoardSquare{ 'd', '5' }] = 1;
				bb[BoardSquare{ 'd', '6' }] = 1;
				bb[BoardSquare{ 'c', '4' }] = 1;
				bb[BoardSquare{ 'c', '5' }] = 1;
				bb[BoardSquare{ 'c', '6' }] = 1;

				showShift<directions::Up>(bb, 0);
				showShift<directions::Up>(bb, 3);
				showShift<directions::Down>(bb, 4);
				showShift<directions::Left>(bb, 5);
				showShift<directions::Right>(bb, 2);
			}

			void allMasks()
			{
				BoardSquare bs('g', '2');

				while (true) {
					char file, rank;

					cout << "Enter board square (use file and rank): ";
					cin >> file >> rank;
					bs = BoardSquare{ file, rank };

					// --- Lateral Line Masks ---
					showMask<directions::Horizontal>(bs);
					showMask<directions::Vertical>(bs);
					showMask<directions::Lateral>(bs);

					// --- Diagonal Line Masks ---
					showMask<directions::MainDiagonal>(bs);
					showMask<directions::OffDiagonal>(bs);
					showMask<directions::Diagonal>(bs);

					// --- Lateral Ray Masks ---
					showMask<directions::Up>(bs);
					showMask<directions::Down>(bs);
					showMask<directions::Left>(bs);
					showMask<directions::Right>(bs);

					// --- Diagonal Ray Masks ---
					showMask<directions::UR>(bs);
					showMask<directions::UL>(bs);
					showMask<directions::DL>(bs);
					showMask<directions::DR>(bs);
				}
			}
		} // namespace bitboard

		namespace piece
		{
			void allToFen()
			{
				cout
					<< forge::pieces::empty << ' '
					<< forge::pieces::whiteKing << ' '
					<< forge::pieces::whiteQueen << ' '
					<< forge::pieces::whiteBishop << ' '
					<< forge::pieces::whiteKnight << ' '
					<< forge::pieces::whiteRook << ' '
					<< forge::pieces::whitePawn << ' '
					<< forge::pieces::blackKing << ' '
					<< forge::pieces::blackQueen << ' '
					<< forge::pieces::blackBishop << ' '
					<< forge::pieces::blackKnight << ' '
					<< forge::pieces::blackRook << ' '
					<< forge::pieces::blackPawn << ' ' << '\n';
			}
		} // namespace pieces

		namespace direction
		{
			template <typename DIRECTION_T>
			void showDir(BoardSquare from, BoardSquare to)
			{
				cout << __FUNCTION__ << " is no longer implemented\n";
				///cout << from << ' ' << DIRECTION_T::str() << ' ' << to << ' '
				///	<< (directions::isPointingTo<DIRECTION_T>(from, to) ? "true" : "false") << endl;
			}

			void pointingTo()
			{
				BoardSquare from{ 5, 3 };
				BoardSquare to{ 2, 3 };

				showDir<directions::Up>(from, to);
				showDir<directions::Down>(from, to);
				showDir<directions::Left>(from, to);
				showDir<directions::Right>(from, to);
				showDir<directions::UR>(from, to);
				showDir<directions::UL>(from, to);
				showDir<directions::DL>(from, to);
				showDir<directions::DR>(from, to);
			}
		}

		void keyboardController()
		{
			ChessMatch match;

			match.makeWhiteController<KeyboardController>();
			match.makeBlackController<KeyboardController>();

			match.makeView<TextView>();

			match.position().clear();
			match.position().board().place<pieces::BlackPawn>(BoardSquare{ 'b', '3' });

			//cout << "You entered " << m << ' ' 
			//	<< (m.isPartial() ? "partial" : "full") << ' '
			//	<< (m.isPromotion() ? "promotion" : "") << " move" << '\n';
			match.runGame();
		}

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

		void stopWatch()
		{
			StopWatch sw;

			sw.reset(chrono::seconds(8));

			cout << sw << '\n';

			sw.resume();

			this_thread::sleep_for(chrono::seconds(2));

			sw.pause();

			this_thread::sleep_for(chrono::seconds(2));

			sw.resume();

			while (sw.elapsed() < chrono::seconds(20)) {
				cout << sw << '\n';

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

			b.place<pieces::Rook>(Square{ 'a', '8' }, BLACK);
			b.place<pieces::Knight>(Square{ 'b', '8' }, BLACK);
			b.place<pieces::Bishop>(Square{ 'c', '8' }, BLACK);
			b.place<pieces::Queen>(Square{ 'd', '8' }, BLACK);
			b.place<pieces::BlackKing>(Square{ 'e', '8' });	// Kings already here
			b.place<pieces::Bishop>(Square{ 'f', '8' }, BLACK);
			b.place<pieces::Knight>(Square{ 'g', '8' }, BLACK);
			b.place<pieces::Rook>(Square{ 'h', '8' }, BLACK);

			b.place<pieces::BlackPawn>(Square{ 'a', '7' });
			b.place<pieces::BlackPawn>(Square{ 'b', '7' });
			b.place<pieces::BlackPawn>(Square{ 'c', '7' });
			b.place<pieces::BlackPawn>(Square{ 'd', '7' });
			b.place<pieces::BlackPawn>(Square{ 'e', '7' });
			b.place<pieces::BlackPawn>(Square{ 'f', '7' });
			b.place<pieces::BlackPawn>(Square{ 'g', '7' });
			b.place<pieces::BlackPawn>(Square{ 'h', '7' });

			b.place<pieces::WhitePawn>(Square{ 'a', '2' });
			b.place<pieces::WhitePawn>(Square{ 'b', '2' });
			b.place<pieces::WhitePawn>(Square{ 'c', '2' });
			b.place<pieces::WhitePawn>(Square{ 'd', '2' });
			b.place<pieces::WhitePawn>(Square{ 'e', '2' });
			b.place<pieces::WhitePawn>(Square{ 'f', '2' });
			b.place<pieces::WhitePawn>(Square{ 'g', '2' });
			b.place<pieces::WhitePawn>(Square{ 'h', '2' });

			b.place<pieces::Rook>(Square{ 'a', '1' }, WHITE);
			b.place<pieces::Knight>(Square{ 'b', '1' }, WHITE);
			b.place<pieces::Bishop>(Square{ 'c', '1' }, WHITE);
			b.place<pieces::Queen>(Square{ 'd', '1' }, WHITE);
			b.place<pieces::WhiteKing>(Square{ 'e', '1' });	// Kings already here
			b.place<pieces::Bishop>(Square{ 'f', '1' }, WHITE);
			b.place<pieces::Knight>(Square{ 'g', '1' }, WHITE);
			b.place<pieces::Rook>(Square{ 'h', '1' }, WHITE);

			b.print();
		}

		namespace piece_moves
		{
			void king()
			{
				templatePiece<pieces::King>();
			}

			void queen()
			{
				templatePiece<pieces::Queen>();
			}

			void bishop()
			{
				templatePiece<pieces::Bishop>();
			}

			void knight()
			{
				templatePiece<pieces::Knight>();
			}

			void rook()
			{
				templatePiece<pieces::Rook>();
			}

			void pawn()
			{
				templatePiece<pieces::WhitePawn>();

				cout << "Press any key to continue...";
				cin.get();

				templatePiece<pieces::BlackPawn>();
			}
		} // namespace piece_moves
		
		namespace fen {
			void allToFen()
			{
				for (const auto& pos : positions) {
					pos.board().printMini();

					cout << pos.toFEN() << endl << endl;
				}
			}

			void allFromFen()
			{
				for (const auto& pos : positions) {
					string fen = pos.toFEN();
					cout << fen << endl;

					Position pos2;
					pos2.fromFEN(fen);
					
					guten::grids::GridView grid;
					grid.setGridCols(2);

					grid.push(pos.board().getCheckerBoard().drawMini());
					grid.push(pos2.board().getCheckerBoard().drawMini());

					grid.toMatrix().print();
				}
			}
		} // namespace fen

		namespace pins
		{
			template<typename DIRECTION_T>
			void printIsPinPossible(const Board & b, bool isKingWhite) {
				MoveGenerator2 moveGen;

				BitBoard theirRays = b.directionals<DIRECTION_T>();

				if (is_base_of<directions::Lateral, DIRECTION_T>()) {
					theirRays |= b.laterals();
				}

				bool isPinPossible =
					moveGen.isPinPossible<directions::Up>(b.laterals() & b.blacks(), b.blockers());

			}

			void isPinPossible()
			{
				Position p;

				Board & b = p.board();

				b.place<pieces::BlackKing>(BoardSquare{ 'a', '8' });
				b.place<pieces::WhiteKing>(BoardSquare{ 'e', '4' });
				b.place<pieces::Rook>(BoardSquare{ 'e', '8' }, BLACK);
				b.place<pieces::WhitePawn>(BoardSquare{ 'e', '6' });
				b.place<pieces::Bishop>(BoardSquare{ 'g', '6' }, BLACK);
				b.place<pieces::WhitePawn>(BoardSquare{ 'f', '5' });

				b.print();

				MoveGenerator2 moveGen;
				moveGen.generate(p);
			}

			void absolutePins()
			{
				Position p;

				Board & b = p.board();
				//b.place<BlackKing>(BoardSquare{ 'e', '8' });
				//b.place<WhiteKing>(BoardSquare{ 'c', '1' });
				//b.place<Rook>(BoardSquare{ 'e', '6' }, BLACK);
				//b.place<Queen>(BoardSquare{ 'e', '4' }, WHITE);

				//b.place<BlackKing>(BoardSquare{ 'c', '8' });
				//b.place<WhiteKing>(BoardSquare{ 'g', '1' });
				//b.place<Queen>(BoardSquare{ 'e', '6' }, BLACK);
				//b.place<Bishop>(BoardSquare{ 'h', '3' }, WHITE);

				//b.place<Rook>(BoardSquare{ 'd', '6' }, BLACK);
				//b.place<Bishop>(BoardSquare{ 'd', '8' }, WHITE);
				//b.place<BlackKing>(BoardSquare{ 'd', '2' });

				//b.place<BlackKing>(BoardSquare{ 'b', '7' });
				//b.place<WhiteKing>(BoardSquare{ 'f', '1' });
				//b.place<BlackPawn>(BoardSquare{ 'd', '5' });
				//b.place<Queen>(BoardSquare{ 'f', '3' }, WHITE);

				//b.place<BlackKing>(BoardSquare{ 'b', '5' });
				//b.place<WhiteKing>(BoardSquare{ 'f', '1' });
				//b.place<BlackPawn>(BoardSquare{ 'd', '5' });
				//b.place<Queen>(BoardSquare{ 'h', '5' }, WHITE);

				//b.place<BlackKing>(BoardSquare{ 'b', '7' });
				//b.place<WhiteKing>(BoardSquare{ 'f', '1' });
				//b.place<BlackPawn>(BoardSquare{ 'e', '4' });
				//b.place<Queen>(BoardSquare{ 'f', '3' }, WHITE);

				//b.place<BlackKing>(BoardSquare{ 'd', '8' });
				//b.place<WhiteKing>(BoardSquare{ 'f', '1' });
				//b.place<BlackPawn>(BoardSquare{ 'd', '4' });
				//b.place<Queen>(BoardSquare{ 'd', '1' }, WHITE);

				b.place<Queen>(BoardSquare{ 'd', '8' }, BLACK);
				b.place<Rook>(BoardSquare{ 'd', '4' }, WHITE);
				b.place<WhiteKing>(BoardSquare{ 'd', '1' });
				b.place<BlackKing>(BoardSquare{ 'a', '8' });

				b.print();

				//p.move<WhiteKing>(Move{ b.whiteKing(), b.whiteKing().rightOne() });	// Make a move as white
				// *** Now its blacks turn.

				cout << (p.moveCounter().isWhitesTurn() ? "Whites" : "Blacks") << " turn" << endl;

				MoveGenerator2 gen;
				MoveList legals = gen.generate(p);

				for (const auto & pair : legals) {
					pair.position.board().printMini();
					cout << endl;
				}

				cout << "Generated " << legals.size() << " legal moves" << endl;
				legals.print();
			}
		} // namespace pins

		namespace movegen
		{
			void genThreatBoard()
			{
				MoveGenerator2 movegen;

				for (const Position & pos : positions) {
					movegen.generate(pos);

					guten::boards::CheckerBoard cb = pos.board().getCheckerBoard();

					// Highlight threats
					BitBoard threats = movegen.getThreats();
					cb.highlight(threats);

					cb.print();
					cout << "Threats: " << threats << endl;
				}
			}

			void genAttackRay()
			{
				Board b;
				b.place<pieces::Rook>(BoardSquare{ 'e', '2' }, BLACK);
				b.place<pieces::BlackPawn>(BoardSquare{ 'c', '2' });
				b.place<pieces::WhitePawn>(BoardSquare{ 'c', '5' });
				b.place<pieces::Bishop>(BoardSquare{ 'e', '7' }, WHITE);
				b.place<pieces::Queen>(BoardSquare{ 'e', '4' }, WHITE);
				b.place<pieces::Knight>(BoardSquare{ 'f', '8' }, WHITE);

				BitBoard threats = forge::Threats::genThreats(b, b.whites());

				b.printMini();

				guten::boards::CheckerBoard cb;

				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						if (threats[BoardSquare{ row, col }])
							cb.highlight(guten::Point{ row, col });
					}
				}
				cb.print();
			}

			void findKingAttackers()
			{
				for (const Position & p : positions) {
					KingAttackers pair;

					const Board & b = p.board();
					pair = KingAttackers::findKingAttackers(b, b.whiteKing(), b.blacks(), b.whites());

					guten::boards::CheckerBoard cb = b.getCheckerBoard();

					if (pair.size() >= 1)
						cb.highlight(pair.attacker0().square.row(), pair.attacker0().square.col());
					if (pair.size() >= 2)
						cb.highlight(pair.attacker1().square.row(), pair.attacker1().square.col());

					cb.print();
					cout << "White King: ";
					pair.print(b);
					cout << endl << endl;

					pair = KingAttackers::findKingAttackers(b, b.blackKing(), b.whites(), b.blacks());
					cb = b.getCheckerBoard();
					if (pair.size() >= 1)
						cb.highlight(pair.attacker0().square.row(), pair.attacker0().square.col());
					if (pair.size() >= 2)
						cb.highlight(pair.attacker1().square.row(), pair.attacker1().square.col());

					cb.print();
					cout << "Black King: ";
					pair.print(b);
					cout << endl << endl;
				}
			}

			void genKingMoves()
			{
				MoveGenerator2 gen;

				for (const auto & pos : positions) {
					cout << endl << "=== It is "
						<< (pos.moveCounter().isWhitesTurn() ? "whites" : "blacks") << " turn ===\n";

					MoveList legals = gen.generate(pos);

					auto cb = pos.board().getCheckerBoard();
					cb.highlight(gen.getThreats());
					cb.print();

					cout << "There are " << legals.size() << " legal moves:\n";
					for (const auto & legal : legals) {
						cout << legal.move << endl;
						legal.position.board().printMini();
					}
				}
			}
		} // namespace movegen

		void moveKings()
		{
			ChessMatch m;
			m.position().clear();

			m.makeWhiteController<KeyboardController>();
			m.makeBlackController<KeyboardController>();
			m.makeView<TextView>();

			m.runGame();
		}

		void legalMoveGenerator()
		{
			///const forge::Position& p = positions.back();
			///for (const auto & p : positions) 
			for (int i = positions.size() - 13; i >= 0 && i < positions.size(); i++) 
			{
				const forge::Position& p = positions.at(i);
				const forge::Board & b = p.board();
				
				forge::MoveGenerator2 mg2;
				forge::MoveList moves =
					//forge::MoveGenerator::generateLegalMoves(p);
					mg2.generate(p);

				guten::boards::CheckerBoard cb = b.getCheckerBoard();
				for (const auto & legal : moves) {
					BoardSquare dst = legal.move.to();
					cb.highlight(dst.row(), dst.col());
				}
				cb.print();

				cout << (p.moveCounter().isWhitesTurn() ? "Whites" : "Blacks") << "	turn" << endl;
				cout << moves.size() << " legal moves generated" << endl;

				guten::grids::GridView gridView;
				gridView.setGridCols(8);

				for (int i = 0; i < moves.size(); i++) {
					const auto & elem = moves.at(i);

					guten::core::Matrix mini = elem.position.board().getMiniBoard();
					BoardSquare from = elem.move.from();
					BoardSquare to = elem.move.to();
					mini.at(from.row(), from.col()).color.setbg(guten::color::cyan);
					mini.at(to.row(), to.col()).color.setbg(guten::color::lightcyan);

					guten::core::Matrix miniText;
					guten::Size sz = mini.size();
					sz.height += 2;
					miniText.resize(sz);
					mini.copyTo(miniText, guten::Point{ 2, 0 });
					
					{
						stringstream ss;
						ss << elem.move.toLAN();
						guten::draw::putText(miniText, ss.str(), guten::Point{ 0, 0 });
					}

					{
						stringstream ss;
						ss << (AttackChecker::isAttacked(elem.position.board(), elem.position.moveCounter().isBlacksTurn()) ?
							"Illegal" : "Legal");
						guten::draw::putText(miniText, ss.str(), guten::Point{ 1, 0 });
					}

					gridView.push(miniText);
				}
				cout << '\n';

				gridView.toMatrix().print();
				auto flags = cout.flags();
				cout << setfill('=') << setw(40) << '\0' << endl;
				cout.flags(flags);
			}
		}

		void isAttacked()
		{
			forge::Position pos;
			forge::Board & b = pos.board();

			pos.clear();

			forge::BoardSquare k{ 'c', '4' };
			b.place<pieces::BlackKing>(k);
			//b.placeWhiteRook(k.down(2));
			//b.placeWhiteBishop(k.downLeft(2));
			//b.placeWhiteQueen(k.down(2));
			//b.placeWhiteKnight(k.knight7());
			//b.placeWhitePawn(k.downLeft());

			b.printMini();

			cout << "--- Black King ---\n"
				<< "\tIs Attacked by:\n"
				//<< "Rook/Queen: " << AttackChecker::isAttackedByRook(b, b.blackKing()) << '\n'
				//<< "Bishop/Queen: " << AttackChecker::isAttackedByBishop(b, b.blackKing()) << '\n'
				//<< "Knights: " << AttackChecker::isAttackedByKnight(b, b.blackKing()) << '\n'
				//<< "Pawns: " << AttackChecker::isAttackedByPawn(b, b.blackKing()) << '\n'
				<< "Is King Attacked: " << AttackChecker::isAttacked(b, b.blackKing()) << '\n'
				<< '\n';
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

			Move m1;
			m1.from(BoardSquare{ 'f', '5' });
			m1.to(BoardSquare{ 'e', '4' });
			m1.promotion(pieces::whiteQueen);
			cout << "m1 = " << m1 << '\n';
		}

		void randomSolver()
		{
			forge::RandomSolver solver;
		}

		void chessMatch()
		{
			forge::ChessMatch match;

			match.reset();
			//match.position().fromFEN("1RK5/k6p/4Q3/p4n1P/8/p5p1/8/2B1NBNR b - - 3 112");

			match.clock().synchronize(
				chrono::minutes(5),
				chrono::minutes(5),
				chrono::seconds(2),
				chrono::seconds(2)
			); // Clock is still paused

			auto whiteController =
				//make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();
				make_unique<MCTS_Solver>();
				//make_unique<KeyboardController>();
			 
			auto blackController =
				//make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();
				make_unique<MCTS_Solver>();
				//make_unique<KeyboardController>();

			whiteController->makeHeuristic<
				//RandomHeuristic
				//ApplePieHeuristic
				NeuralNetworkHeuristic
			>();
			blackController->makeHeuristic<
				//RandomHeuristic
				//ApplePieHeuristic
				NeuralNetworkHeuristic
			>();

			match.whiteController() = std::move(whiteController);
			match.blackController() = std::move(blackController);

			match.makeView<TextView>();
			match.runGame();
		}

		void performanceTester()
		{
			forge::PerformanceTester tester;

			auto whiteSolver =
				//make_unique<RandomSolver>();
				make_unique<MinimaxSolver>();

			auto blackSolver =
				//make_unique<RandomSolver>();
				make_unique<MinimaxSolver>();

			whiteSolver->makeHeuristic<forge::ApplePieHeuristic>();
			blackSolver->makeHeuristic<forge::ApplePieHeuristic>();

			tester.whiteSolver() = std::move(whiteSolver);
			tester.blackSolver() = std::move(blackSolver);

			tester.view() = make_unique<TextView>();

			tester.runGame();
		}

		void nodeIterator()
		{
			MiniMaxNode root;
			root.position().reset();

			MiniMaxNode::iterator it = root.begin();
			it.setDepthLimit(3);

			while (it != root.end()) {
				(*it).position().board().print();

				++it;

				cin.get();
			}
		}

		void heuristic()
		{
			Position position;
			position.reset();

			unique_ptr<HeuristicBase> ptr =
				//make_unique<ApplePieHeuristic>();
				make_unique<RandomHeuristic>();

			while (true) {
				cout << ptr->eval(position) << ' ';
				cin.get();
			}
		}

		void solver()
		{
			Position position;
			position.reset();

			position.board().print();

			unique_ptr<SolverBase> solverPtr =
				//make_unique<RandomSolver>();
				make_unique<MinimaxSolver>();

			solverPtr->makeHeuristic<RandomHeuristic>();

			solverPtr->reset();

			while (true) {
				MovePositionPair pair = solverPtr->getMove(position);

				position = pair.position;

				position.board().print();
				//cout << "Press any key...";
				//cin.get();
			}
		}

		void insufficientMaterial()
		{
			MiniMaxNode n;
			Position & pos = n.position();
			pos.clear();

			//pos.applyMoveFast(Move{ pos.board().whiteKing(), pos.board().whiteKing().upOne() });
			//pos.board().placeBlackPawn(BoardSquare('a', '2'));
			//pos.board().placeWhiteBishop(BoardSquare('e', '5'));
			//pos.board().placeWhiteKnight(BoardSquare('e', '4'));
			//pos.board().placeWhiteKnight(BoardSquare('d', '4'));
			//pos.board().placeRook(BoardSquare('e', '4'), true);
			pos.board().print();

			GameState gs;
			gs(n);

			cout << "Game state: " << gs << '\n';
		}

		void gameState()
		{
			forge::Position pos;
			pos.clear();

			pos.board().place<pieces::Rook>(forge::BoardSquare{ 'e', '4' }, BLACK);

			pos.board().print();

			GameState state;

			//state(pos);

			cout << state << '\n';
		}

		void drawByRepetition()
		{
			ChessMatch match;
			///match.makeWhiteController<ReplayController>();
			///match.makeBlackController<ReplayController>();

			unique_ptr<ReplayController> whitePtr = make_unique<ReplayController>();
			unique_ptr<ReplayController> blackPtr = make_unique<ReplayController>();

			ReplayController & white = *whitePtr;
			ReplayController & black = *blackPtr;

			white.push(Move{ BoardSquare{'d', '2'}, BoardSquare{'d', '4'} });	// queen's pawn
			black.push(Move{ BoardSquare{'d', '7'}, BoardSquare{'d', '5'} });	// queen's pawn
			white.push(Move{ BoardSquare{'d', '1'}, BoardSquare{'d', '3'} });	// white queen
			black.push(Move{ BoardSquare{'d', '8'}, BoardSquare{'d', '6'} });	// black queen
			white.push(Move{ BoardSquare{'d', '3'}, BoardSquare{'d', '1'} });	// white undo
			black.push(Move{ BoardSquare{'d', '6'}, BoardSquare{'d', '8'} });	// black undo

			white.push(Move{ BoardSquare{'d', '1'}, BoardSquare{'d', '3'} });	// white queen
			black.push(Move{ BoardSquare{'d', '8'}, BoardSquare{'d', '6'} });	// black queen
			white.push(Move{ BoardSquare{'d', '3'}, BoardSquare{'d', '1'} });	// white undo
			black.push(Move{ BoardSquare{'d', '6'}, BoardSquare{'d', '8'} });	// black undo

			white.push(Move{ BoardSquare{'d', '1'}, BoardSquare{'d', '3'} });	// white queen
			black.push(Move{ BoardSquare{'d', '8'}, BoardSquare{'d', '6'} });	// black queen
			white.push(Move{ BoardSquare{'d', '3'}, BoardSquare{'d', '1'} });	// white undo
			black.push(Move{ BoardSquare{'d', '6'}, BoardSquare{'d', '8'} });	// black undo

			cout << "White: " << white.size() << '\n'
				<< "Black: " << black.size() << '\n';

			match.setWhiteController(std::move(whitePtr));
			match.setBlackController(std::move(blackPtr));

			white.pauseBeforeReturn();
			black.pauseBeforeReturn();

			match.makeView<TextView>();

			match.runGame();
		}

		void logger()
		{
			PerformanceLogger logger;

			filesystem::path currDir = filesystem::current_path();
			logger.start(
				"Some Solver",
				"Some Variant",
				"Some Evaluation Function",
				"v0.0");

			logger.log(50, 0.5, 2, 0.6, chrono::milliseconds(234));
			logger.log(523, 0.55, 25, 0.36, chrono::milliseconds(2234));
		}

		namespace weights
		{
			void applePie()
			{
				ApplePieWeights w;

				// --- Material ---
				w.queenMaterial = 1;
				w.rookMaterial = 2;
				w.bishopMaterial = 3;
				w.knightMaterial = 4;
				w.pawnMaterial = 5;
				w.bishopPair = 6;
				w.oppositeBishop = 7;

				// --- Mobility ---
				w.queenMobility = 8;
				w.rookMobility = 9;
				w.bishopMobility = 10;
				w.knightMobility = 11;
				w.pawnMobility = 12;
				w.kingMobility = 13;

				// --- Piece Square Table Bonus ---
				w.queenPSTB = { {
					1, 1, 1, 1, 1, 1, 1, 1,
					2, 2, 2, 2, 2, 2, 2, 2,
					3, 3, 3, 3, 3, 3, 3, 3,
					4, 4, 4, 4, 4, 4, 4, 4,
					5, 5, 5, 5, 5, 5, 5, 5,
					6, 6, 6, 6, 6, 6, 6, 6,
					7, 7, 7, 7, 7, 7, 7, 7,
					8, 8, 8, 8, 8, 8, 8, 8, } };
				w.rookPSTB = w.queenPSTB;
				w.bishopPSTB = w.queenPSTB;
				w.knightPSTB = w.queenPSTB;
				w.pawnPSTB = w.queenPSTB;
				w.kingPSTB = w.queenPSTB;

				// --- Defended Pieces ---
				w.queensDefended = 20;
				w.rooksDefended = 21;
				w.bishopsDefended = 22;
				w.knightsDefended = 23;
				w.pawnsDefended = 24;
				w.kingsDefended = 25;

				WeightsArchive ar;
				w.serialize(ar);

				auto v = ar.to<vector<heuristic_t>>();

				cout << "Weights: ";
				for (const auto & elem : v) {
					cout << elem << '\t';
				}

				ApplePieWeights w2;

				w2.parse(ar);

				cout << "\nAre w and w2 equal? " << (w == w2) << '\n';
			}
		} // namespace weights

		namespace ai
		{
			void playApplePie()
			{
				forge::ChessMatch match;

				// Set Controllers/Solvers
				{
					// --- Make Heuristics and set their weights ---
					unique_ptr<forge::ApplePieHeuristic> wHeur = make_unique<forge::ApplePieHeuristic>();
					unique_ptr<forge::ApplePieHeuristic> bHeur = make_unique<forge::ApplePieHeuristic>();

					// Set weights manually
					auto & w = wHeur->weights();
					w.queenMaterial = 900;
					w.rookMaterial = 500;
					w.bishopMaterial = 300;
					w.knightMaterial = 300;
					w.pawnMaterial = 100;

					// Copy same weights to blacks heuristic
					bHeur->weights() = w;

					// *** Now both black and white have weights that have been set.

					// --- Create solvers ---
					unique_ptr<forge::MinimaxSolver> wController = make_unique<forge::MinimaxSolver>();
					unique_ptr<forge::MinimaxSolver> bController = make_unique<forge::MinimaxSolver>();

					// --- Move heuristics into solvers ---
					wController->heuristicPtr() = std::move(wHeur);
					bController->heuristicPtr() = std::move(bHeur);

					// --- Move Solvers into ChessMatch ---
					match.whiteController() = std::move(wController);
					match.blackController() = std::move(bController);

					match.makeView<TextView>();

					match.runGame();
				}
			}
		} // namespace ai
	} // namespace test
} // namespace forge
