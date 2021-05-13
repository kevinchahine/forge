#include "Piece.h"

using namespace std;

namespace forge
{
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

	namespace pieces {
		BitBoard Empty::moves(BoardSquare square)
		{
			return BitBoard(); // Empty BitBoard for Empty square
		}

		BitBoard King::moves(BoardSquare square)
		{
			BitBoard bb;

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

		BitBoard Queen::moves(BoardSquare square)
		{
			return Rook::moves(square) | Bishop::moves(square);
		}

		BitBoard Bishop::moves(BoardSquare square)
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

			return bb;
		}

		BitBoard Knight::moves(BoardSquare square)
		{
			BoardSquare & s = square;	// Alias name

			BitBoard bb;

			if (s.isKnight0InBounds()) bb[s.knight0()] = 1;
			if (s.isKnight1InBounds()) bb[s.knight1()] = 1;
			if (s.isKnight2InBounds()) bb[s.knight2()] = 1;
			if (s.isKnight3InBounds()) bb[s.knight3()] = 1;
			if (s.isKnight4InBounds()) bb[s.knight4()] = 1;
			if (s.isKnight5InBounds()) bb[s.knight5()] = 1;
			if (s.isKnight6InBounds()) bb[s.knight6()] = 1;
			if (s.isKnight7InBounds()) bb[s.knight7()] = 1;

			///BoardSquare base{ 2, 2 };
			///
			///const BitBoard KNIGHTS = 0b00000000'00000000'00000000'00001010'00010001'00000000'00010001'00001010;
			///const BitBoard BLANKER = 0b0;
			///
			///bb = KNIGHTS << (3);	// Horizontal Shift
			///
			///bb = bb << (1 * 8);		// Vertical Shift

			return bb;
		}

		BitBoard Rook::moves(BoardSquare square)
		{
			BitBoard bb;

			const BitBoard TOP = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;
			const BitBoard LEFT = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;

			bb =
				(TOP << (square.row() << 3)) |
				(LEFT << square.col());

			return bb;
		}

		BitBoard WhitePawn::moves(BoardSquare square) const
		{
			BitBoard bb;

			// --- Left Capture ---
			if (square.isLeftFile() == false) {
				bb[square.upLeftOne()] = 1;
			}

			// --- Single Push ---
			bb[square.upOne()] = 1;

			// --- Right Capture ---
			if (square.isRightFile() == false) {
				bb[square.upRightOne()] = 1;
			}

			// --- Double Push ---
			if (square.row() == 6) {
				bb[square.up(2)] = 1;
			}

			return bb;
		}

		BitBoard BlackPawn::moves(BoardSquare square) const
		{
			BitBoard bb;

			// --- Left Capture ---
			if (square.isLeftFile() == false) {
				bb[square.downLeftOne()] = 1;
			}

			// --- Single Push ---
			bb[square.upOne()] = 1;

			// --- Right Capture ---
			if (square.isRightFile() == false) {
				bb[square.downRightOne()] = 1;
			}

			// --- Double Push ---
			if (square.row() == 1) {
				bb[square.down(2)] = 1;
			}

			return bb;
		}
	} // namespace pieces
} // namespace forge