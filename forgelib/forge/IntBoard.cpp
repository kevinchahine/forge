#include "IntBoard.h"

#include "Guten/draw/DrawFunctions.h"

#include <numeric>
#include <iomanip>
#include <array>
#include <string>

using namespace std;

namespace forge
{
	IntBoard IntBoard::operator&(const BitBoard& board)
	{
		IntBoard res;

		for (int row = 0; row < res.nrows(); row++) {
			for (int col = 0; col < res.ncols(); col++) {
				if (board[BoardSquare{ row, col }]) {
					res[row][col] = (*this)[row][col];
				}
			}
		}

		return res;
	}

	guten::core::Matrix IntBoard::toMat() const
	{
		array<array<string, 8>, 8> strings;

		for (size_t row = 0; row < this->nrows(); row++) {
			for (size_t col = 0; col < this->ncols(); col++) {
				strings[row][col] = to_string(arr[row][col]);
			}
		}

		array<size_t, 8> colWidths;

		for (size_t col = 0; col < this->ncols(); col++) {
			for (size_t row = 0; row < this->nrows(); row++) {
				if (strings[row][col].size() > colWidths[col]) {
					colWidths[col] = strings[row][col].size();
				}
			}
		}

		int fullColWidth = accumulate(colWidths.begin(), colWidths.end(), 0) + 8;

		guten::core::Matrix mat{ 8, fullColWidth };
		
		for (int row = 0; row < this->nrows(); row++) {
			int colIter = 0;
			for (int col = 0; col < this->ncols(); col++) {
				guten::draw::putText(mat, strings[row][col], guten::Point{ row, colIter });
				colIter += colWidths[col] + 1;
			}
		}

		return mat;
	}
	
	void IntBoard::print(std::ostream& os) const
	{
		// TODO: use toMat() instead
		//this->toMat().print(0, os);

		for (int row = 0; row < this->nrows(); row++) {
			for (int col = 0; col < this->ncols(); col++) {
				os << setw(5) << (*this)[row][col];
			}
			os << endl;
		}
		os << endl;
	}
} // namespace forge