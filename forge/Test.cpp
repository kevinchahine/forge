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
			void genAttackRay()
			{
				Board b;
				b.place<pieces::Rook>(BoardSquare{ 'e', '2' }, BLACK);
				b.place<pieces::BlackPawn>(BoardSquare{ 'c', '2' });
				b.place<pieces::WhitePawn>(BoardSquare{ 'c', '5' });
				b.place<pieces::Bishop>(BoardSquare{ 'e', '7' }, WHITE);
				b.place<pieces::Queen>(BoardSquare{ 'e', '4' }, WHITE);
				b.place<pieces::Knight>(BoardSquare{ 'f', '8' }, WHITE);

				BitBoard threats = forge::MoveGenHelpers::genThreats(b, b.whites());

				b.printMini();

				guten::boards::CheckerBoard cb;
				cb.drawBackground();
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						if (threats[BoardSquare{ row, col }])
							cb.highlight(guten::Point{ row, col });
					}
				}
				cb.print();

				cout << "Queen\n";

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
			forge::Position p;
			forge::Board & b = p.board();

			p.reset();
			p.clear();

			///b.placeAllPieces();
			forge::BoardSquare s{ 5, 4 };
			b.place<pieces::Knight>(s, WHITE);
			b.place<pieces::Queen>(s.right(), WHITE);
			b.place<pieces::Bishop>(s.up(2), BLACK);
			b.place<pieces::WhitePawn>(s.left(2));
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

			match.clock().synchronize(
				chrono::minutes(5),
				chrono::minutes(5),
				chrono::seconds(2),
				chrono::seconds(2)
			); // Clock is still paused

			auto whiteController =
				make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();

			auto blackController =
				make_unique<RandomSolver>();
				//make_unique<MinimaxSolver>();

			whiteController->makeHeuristic<
				//RandomHeuristic
				ApplePieHeuristic
			>();
			blackController->makeHeuristic<
				//RandomHeuristic
				ApplePieHeuristic
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
					wController->heuristicPtr() = move(wHeur);
					bController->heuristicPtr() = move(bHeur);

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
