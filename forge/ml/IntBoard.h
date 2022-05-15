#pragma once

#include "BoardSquare.h"
#include "BitBoard.h"

#include "Guten/core/Matrix.h"

#include <iostream>

namespace forge
{
	class IntBoard
	{
	public:
		IntBoard operator&(const BitBoard& board);

		int* operator[](size_t row) { return arr[row]; }
		const int* operator[](size_t row) const { return arr[row]; }

		int& operator[](BoardSquare bs) { return arr[bs.row()][bs.col()]; }
		const int& operator[](BoardSquare bs) const { return arr[bs.row()][bs.col()]; }

		size_t nrows() const { return 8; }
		size_t ncols() const { return 8; }

		guten::core::Matrix toMat() const;

		void print(std::ostream& os = std::cout) const;

	public:
		int arr[8][8] = { 0 };
	};
} // namespace forge
