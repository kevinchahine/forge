#include "Piece.h"

using namespace std;

namespace forge
{
	namespace pieces {
		char Piece::getCh() const
		{
			// Look up table
			// converts piece value to a printable character
			// index - piece value
			// output - character 
			static const uint8_t pieceValToChar[] = {
				' ',	// 0  Empty
				'K',	// 1  White
				'Q',	// 2  White
				'B',	// 3  White
				'N',	// 4  White
				'R',	// 5  White
				'p',	// 6  White
			};

			Piece p = *this;

			p.makeWhite();

			return pieceValToChar[p.m_val.to_ulong()];
		}

		void Piece::setCh(char ch, bool isWhite)
		{
			ch = tolower(ch);

			switch (ch) {
			case ' ': 	m_val = EMPTY;			break;
			case 'k': 	m_val = WHITE_KING;		break;
			case 'q': 	m_val = WHITE_QUEEN;	break;
			case 'b': 	m_val = WHITE_BISHOP;	break;
			case 'n': 	m_val = WHITE_KNIGHT;	break;
			case 'r': 	m_val = WHITE_ROOK;		break;
			case 'p': 	m_val = WHITE_PAWN;		break;
			default:
#ifdef _DEBUG
				cout << "Error: " << __FUNCTION__ << " unknown character " << ch << '\n';
#endif 
				break;
			}

			// What color should the piece be?
			if (!isWhite) {
				// It should be black
				this->makeBlack();
			}
		}

		// -------------------------------- Individual Pieces ---------------------

		BitBoard King::pushMask(BoardSquare square)
		{
			BitBoard bb;

			// TODO: Optimize: Make this with only bitwise operations. No "if"s
			// Use shift operations
			// --- Top 3 squares ---
			if (square.isTopRank() == false) {
				bb[square.upOne()] = 1;

				if (square.isLeftFile() == false) {
					bb[square.upLeftOne()] = 1;
				}
				if (square.isRightFile() == false) {
					bb[square.upRightOne()] = 1;
				}
			}

			// --- Middle 3 squares ---
			if (square.isLeftFile() == false) {
				bb[square.leftOne()] = 1;
			}
			if (square.isRightFile() == false) {
				bb[square.rightOne()] = 1;
			}

			// --- Bottom 3 squares ---
			if (square.isBotRank() == false) {
				bb[square.downOne()] = 1;

				if (square.isLeftFile() == false) {
					bb[square.downLeftOne()] = 1;
				}
				if (square.isRightFile() == false) {
					bb[square.downRightOne()] = 1;
				}
			}

			return bb;
		}

		BitBoard Queen::pushMask(BoardSquare square)
		{
			return Rook::pushMask(square) | Bishop::pushMask(square);
		}

		BitBoard Bishop::pushMask(BoardSquare square)
		{
			BitBoard bb;

			const BitBoard POS_DIAGONAL = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;
			const BitBoard NEG_DIAGONAL = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

			int mainShift = 7 - (square.row() + square.col());
			int inverseShift = square.row() - square.col();

			if (mainShift > 0) {
				bb |= (POS_DIAGONAL >> (mainShift << 3));
			}
			else {
				bb |= (POS_DIAGONAL << (-mainShift << 3));
			}

			if (inverseShift > 0) {
				bb |= (NEG_DIAGONAL << (inverseShift << 3));
			}
			else {
				bb |= (NEG_DIAGONAL >> (-inverseShift << 3));
			}

			bb[square] = 0;	// Clear bit where piece is standing

			return bb;
		}

		BitBoard Knight::pushMask(BoardSquare square)
		{
			return BitBoard::mask<directions::LShape>(square);
		}

		BitBoard Rook::pushMask(BoardSquare square)
		{
			BitBoard bb;

			const BitBoard TOP = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;
			const BitBoard LEFT = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

			bb =
				(TOP << (square.row() << 3)) |
				(LEFT << square.col());

			bb[square] = 0;	// Clear bit where piece is standing

			return bb;
		}

		BitBoard WhitePawn::pushMask(BoardSquare square)
		{
			BitBoard bb;

			// --- Single Push ---
			bb[square.upOne()] = 1;

			// --- Double Push ---
			if (square.row() == 6) {
				bb[square.up(2)] = 1;
			}

			return bb;
		}

		BitBoard WhitePawn::captureMask(BoardSquare square)
		{
			BitBoard bb;

			// *** Does not need vertical bounds checking ***
			// *** because pawns can't be on top or bottom rank ***

			// TODO: Optimize: Try to use bit shifts instead of if 

			// --- Left Capture ---
			if (square.isLeftFile() == false) {
				bb[square.upLeftOne()] = 1;
			}

			// --- Right Capture ---
			if (square.isRightFile() == false) {
				bb[square.upRightOne()] = 1;
			}

			return bb;
		}

		BitBoard BlackPawn::pushMask(BoardSquare square)
		{
			BitBoard bb;

			// --- Single Push ---
			bb[square.downOne()] = 1;

			// --- Double Push ---
			if (square.row() == 1) {
				bb[square.down(2)] = 1;
			}

			return bb;
		}

		BitBoard BlackPawn::captureMask(BoardSquare square)
		{
			BitBoard bb;

			// *** Does not need vertical bounds checking ***
			// *** because pawns can't be on top or bottom rank ***

			// TODO: Optimize: Try to use bit shifts instead of if 

			// --- Left Capture ---
			if (square.isLeftFile() == false) {
				bb[square.downLeftOne()] = 1;
			}

			// --- Right Capture ---
			if (square.isRightFile() == false) {
				bb[square.downRightOne()] = 1;
			}

			return bb;
		}

		// --- These definitions must be place here below other overloads to compile properly ---

		BitBoard Piece::pushMask(BoardSquare square) const
		{
			// Pawn is first because it is most common piece
			if (isPawn()) {
				if (isWhite())	return WhitePawn::pushMask(square);
				else			return BlackPawn::pushMask(square);
			}
			if (isQueen())		return Queen::pushMask(square);
			if (isBishop())		return Bishop::pushMask(square);
			if (isKnight())		return Knight::pushMask(square);
			if (isRook())		return Rook::pushMask(square);
			if (isKing())		return King::pushMask(square);
#ifdef _DEBUG
			cout << "Error: " << __FILE__ << " line " << __LINE__
				<< " Piece type could not be identified. Square might be empty.\n";
#endif // _DEBUG
		}

		BitBoard Piece::captureMask(BoardSquare square) const
		{
			// Pawn is first because it is most common piece
			if (isPawn()) {
				if (isWhite())	return WhitePawn::captureMask(square);
				else			return BlackPawn::captureMask(square);
			}
			if (isQueen())		return Queen::captureMask(square);
			if (isBishop())		return Bishop::captureMask(square);
			if (isKnight())		return Knight::captureMask(square);
			if (isRook())		return Rook::captureMask(square);
			if (isKing())		return King::captureMask(square);

#ifdef _DEBUG
			cout << "Error: " << __FILE__ << " line " << __LINE__
				<< " Piece type could not be identified. Square might be empty.\n";
#endif // _DEBUG
		}

	} // namespace pieces
} // namespace forge