#include "UciScore.h"

#include <boost/optional/optional_io.hpp>

#include <string>

using namespace std;

namespace forge
{
	namespace uci
	{
		ostream & operator<<(ostream& os, const UciScore& score)
		{
			if (score.cp || score.mate || score.lowerbound || score.upperbound) {
				os << "score ";

				if (score.cp)			os << "cp " << score.cp.get() << ' ';
				if (score.mate)			os << "mate " << score.mate.get() << ' ';
				if (score.lowerbound)	os << "lowerbound " << score.lowerbound.get() << ' ';
				if (score.upperbound)	os << "upperbound " << score.upperbound.get() << ' ';
			}

			return os;
		}

		istream& operator>>(istream& is, UciScore& score)
		{
			string token;

			while (is.eof() == false) {
				string token;

				is >> token;

				if (token == "cp")				is >> score.cp;
				else if (token == "mate")		is >> score.mate;
				else if (token == "lowerbound")	is >> score.lowerbound;
				else if (token == "upperbound")	is >> score.upperbound;
			}

			return is;
		}
	} // namespace uci
} // namespace forge