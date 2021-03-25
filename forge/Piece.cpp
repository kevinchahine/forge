#include "Piece.h"

using namespace std;

namespace forge
{
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
			m_val = -m_val;
		}
	}
} // namespace forge