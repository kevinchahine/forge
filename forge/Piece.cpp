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
} // namespace forge