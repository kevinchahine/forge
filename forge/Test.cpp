#pragma once

#include "Test.h"
#include "TextView.h"

#include "Guten/GridView.h"

#include <sstream>

using namespace std;

namespace forge
{
	namespace test
	{
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

		void keyboardController()
		{
			ChessMatch match;

			match.makeWhiteController<KeyboardController>();
			match.makeBlackController<KeyboardController>();

			match.makeView<TextView>();

			match.position().clear();
			match.position().board().placeBlackPawn(BoardSquare{ 'b', '3' });

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
			forge::Position p;
			forge::Board & b = p.board();

			p.reset();
			p.clear();

			///b.placeAllPieces();
			forge::BoardSquare s{ 5, 4 };				
			b.placeWhiteKnight(s);						
			b.placeWhiteQueen(s.right());						
			b.placeBlackBishop(s.up(2));				
			b.placeWhitePawn(s.left(2));				
			(const_cast<forge::MoveCounter &>(p.moveCounter()))++;

			b.print();

			forge::MoveList moves =
				forge::MoveGenerator::generateLegalMoves(p);

			cout << moves.size() << " legal moves generated" << endl;

			guten::grids::GridView gridView;
			gridView.setGridCols(8);

			for (int i = 0; i < moves.size(); i++) {
				const auto & elem = moves.at(i);

				guten::core::Matrix mini = elem.position.board().getMiniBoard();

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
					ss << (AttackChecker::isAttacked(elem.position.board(), elem.position.board().blackKing()) ?
						"Illegal" : "Legal");
					guten::draw::putText(miniText, ss.str(), guten::Point{ 1, 0 });
				}

				gridView.push(miniText);
			}
			cout << '\n';

			gridView.toMatrix().print();
		}

		void isAttacked()
		{
			forge::Position pos;
			forge::Board & b = pos.board();

			pos.clear();

			forge::BoardSquare k{ 'c', '4' };
			b.moveBlackKing(k);
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

			match.clock().synchronize(
				chrono::minutes(5),
				chrono::minutes(5),
				chrono::seconds(2),
				chrono::seconds(2)
			); // Clock is still paused

			auto whiteController =
				//make_unique<RandomSolver>();
				make_unique<MinimaxSolver>();

			auto blackController =
				//make_unique<RandomSolver>();
				make_unique<MinimaxSolver>();
			
			whiteController->makeHeuristic<RandomHeuristic>();
			blackController->makeHeuristic<RandomHeuristic>();

			match.whiteController() = std::move(whiteController);
			match.blackController() = std::move(blackController);

			match.makeView<TextView>();

			match.runGame();
		}

		void nodeIterator()
		{
			Node root;
			root.position().reset();

			Node::iterator it = root.begin();
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
			Node n;
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

			pos.board().placeBlackRook(forge::BoardSquare{ 'e', '4' });

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
	} // namespace test
} // namespace forge