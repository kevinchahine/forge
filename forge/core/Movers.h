#pragma once

#include "forge/core/BoardSquare.h"

#include <iostream>

namespace forge
{
	// --- Forward Declarations ---
	class BoardSquare;

	namespace movers
	{
		// --- Base Classes ---
		class Direction {};

		class LateralMove : public Direction {};
		class DiagonalMove : public Direction {};
		class KnightMove : public Direction {};

		// ---------------------------- LATERAL -------------------------------

		class MoveUp : public LateralMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.upOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false;
			}
		};

		class MoveDown : public LateralMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.downOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false;
			}
		};

		class MoveLeft : public LateralMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.leftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isLeftFile() == false;
			}
		};

		class MoveRight : public LateralMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.rightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isRightFile() == false;
			}
		};

		// ---------------------------- DIAGONAL ------------------------------------

		class MoveUpRight : public DiagonalMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.upRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isRightFile() == false;
			}
		};

		class MoveUpLeft : public DiagonalMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.upLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isLeftFile() == false;
			}
		};

		class MoveDownRight : public DiagonalMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.downRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isRightFile() == false;
			}
		};

		class MoveDownLeft : public DiagonalMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.downLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isLeftFile() == false;
			}
		};

		// ---------------------------- KNIGHTS -------------------------------
		
		class MoveKnight0 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight0();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight0InBounds();
			}
		};

		class MoveKnight1 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight1();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight1InBounds();
			}
		};

		class MoveKnight2 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight2();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight2InBounds();
			}
		};

		class MoveKnight3 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight3();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight3InBounds();
			}
		};

		class MoveKnight4 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight4();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight4InBounds();
			}
		};

		class MoveKnight5 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight5();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight5InBounds();
			}
		};

		class MoveKnight6 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight6();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight6InBounds();
			}
		};

		class MoveKnight7 : public KnightMove
		{
		public:
			static BoardSquare move(const BoardSquare & square) {
				return square.knight7();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight7InBounds();
			}
		};
	} // namespace movers
} // namespace forge