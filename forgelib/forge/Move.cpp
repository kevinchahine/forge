#include "forge/Move.h"

#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

unsigned char my_tolower(unsigned char c) { return std::tolower(c); }

namespace forge
{
	ostream & operator<<(ostream & os, const Move & move)
	{
		os << move.from() << move.to();

		pieces::Piece promotion = move.promotion();

		if (promotion != pieces::empty) {
			os << promotion;
		}

		return os;
	}

	istream & operator>>(std::istream & is, Move & move)
	{
		/*
		good inputs:
		a4
		a4a5
		a7a8q
		a8q
		q			ok just specifying a promotion

		ok inputs (won't cause problems):
		a4a4

		bad inputs:
		a			not a full coordinate
		4			not a full coordinate
		a4a			"
		aq			"
		a4q			a4 isn't a promotional rank
		*/

		string input;
		getline(is, input);

		// Make lower case
		transform(input.begin(), input.end(), input.begin(), my_tolower);

		if (size_t sz = input.size(); sz == 0 || sz > 5) {
			move.setInvalid();
			return is;
		}

		// *** At this point we know that input is between 1 and 5 characters long ***

		if (input.size() == 1) {
			// Must be a promotion nothing else.
			move.promotion(input.at(0));
		}
		else if (input.size() == 2) {
			// Must be a partial move.
			// Either 'to' or 'from' component.
			char file = input.at(0);
			char rank = input.at(1);

			move = Move(BoardSquare{ file, rank }, BoardSquare{ file, rank }, pieces::empty);
		}
		else if (input.size() == 3) {
			// Must be partial move.
			// Must be 'to' component and a promotion.
			char file = input.at(0);
			char rank = input.at(1);
			char prom = input.at(2);

			move.to(file, rank);

			if (move.isValid()) {
				move.promotion(prom);

				if (move.isValid()) {
					move.from(move.to());
				}
			}
		}
		else if (input.size() == 4) {
			// File rank file rank, nothing else
			char fromFile = input.at(0);
			char fromRank = input.at(1);
			char toFile = input.at(2);
			char toRank = input.at(3);

			move.promotion(pieces::empty);

			move.from(fromFile, fromRank);
			
			if (move.isValid()) {
				move.to(toFile, toRank);
			}
		}
		else if (input.size() == 5) {
			// File rank file rank, nothing else
			char fromFile = input.at(0);
			char fromRank = input.at(1);
			char toFile = input.at(2);
			char toRank = input.at(3);
			char prom = input.at(4);

			move.promotion(prom);

			if (move.isValid()) {
				move.from(fromFile, fromRank);

				if (move.isValid()) {
					move.to(toFile, toRank);
				}
			}
		}

		return is;
	}

	Move::Move(const std::string & notation)
	{
		// --- LAN notation ---
		from(notation[0], notation[1]);
		to(notation[2], notation[3]);
		if (notation.size() >= 5)	promotion(notation[4]);
	}

	void Move::from(char file, char rank)
	{
		file = tolower(file);
		rank = tolower(rank);

		if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
			this->from(BoardSquare{ file, rank });
		}
		else {
			this->setInvalid();
		}
	}

	void Move::to(char file, char rank)
	{
		file = tolower(file);
		rank = tolower(rank);

		if (file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8') {
			this->to(BoardSquare{ file, rank });
		}
		else {
			this->setInvalid();
		}
	}

	void Move::promotion(char promotionCh)
	{
		pieces::Piece p;

		switch (tolower(promotionCh))
		{
		case 'q':	p = pieces::whiteQueen;		break;
		case 'r':	p = pieces::whiteRook;		break;
		case 'b':	p = pieces::whiteBishop;	break;
		case 'n':	p = pieces::whiteKnight;	break;
		case ' ':	p = pieces::empty;			break;
#ifdef _DEBUG
		default:
			cout << "Error: " << __FUNCTION__ << " \'" << promotionCh << '\''
				<< " is not a promotion pieces\n";

			this->isInvalid();
#endif
		}

		// Set color of piece
		if (this->to().isTopRank()) {
			p.makeWhite();
		}
		else if (this->to().isBotRank()) {
			p.makeBlack();
		}
		else {
			this->setInvalid();
		}

		this->promotion(p);
	}

	string Move::toLAN() const
	{
		// TODO: Nullmoves should be "0000"

		stringstream ss;

		ss << from() << to();

		pieces::Piece promo = promotion();

		if (promo.isEmpty() == false) {
			ss << promo;
		}

		return ss.str();
	}
	
	size_t Move::hash() const
	{
		return std::hash<std::bitset<16>>{}(m_val);
	}
} // namespace forge