#include "Move.h"

#include <string>
#include <sstream>

using namespace std;

namespace forge
{
	ostream & operator<<(ostream & os, const Move & move)
	{
		os << move.from() << move.to();

		Piece promotion = move.promotion();

		if (promotion != pieces::empty) {
			os << promotion;
		}

		return os;
	}

	istream & operator>>(std::istream & is, Move & move)
	{
		char fromFile;			// 'a' - 'h'
		char fromRank;			// '1' - '8'
		char toFile;			// 'a' - 'h'
		char toRank;			// '1' - '8'

		BoardSquare from;
		BoardSquare to;
		Piece promotion;

		// --- 'from' coordinate (both chars are needed) ---
		is >> fromFile >> fromRank;
		from = BoardSquare{ fromFile, fromRank };

		// --- 'to' coordinate (optional) ---
		// initialize with 'from' coordinates incase they arn't given

		char nextCh = is.peek();
		if (isalpha(nextCh) || isdigit(nextCh)) {
			is >> toFile;

			nextCh = is.peek();
			if (isalpha(nextCh) || isdigit(nextCh)) {
				is >> toRank;
			}
			
			to = BoardSquare{ toFile, toRank };

			// --- 'promotion' (optional) ---
			nextCh = is.peek();
			if (isalpha(nextCh)) {
				is >> promotion;

				// Set color of piece
				if (to.isTopRank()) {
					promotion.makeWhite();
				}
				else if (to.isBotRank()) {
					promotion.makeBlack();
				}
			}
		}
		else {
			// No 'to' coordinate
			to = from;
		}

		move = Move{ from, to, promotion };

		return is;
	}

	string Move::toLAN() const
	{
		stringstream ss;

		ss << from() << to();

		Piece promo = promotion();

		if (promo.isEmpty() == false) {
			ss << promo;
		}

		return ss.str();
	}
} // namespace forge