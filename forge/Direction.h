#pragma once

#include "BoardSquare.h"

namespace forge
{
	namespace directions
	{
		class Direction {};

		// --- Line Directions (Mixin Class)  ---

		class Linear : public Direction {};
		class NonLinear : public Direction {};
		class Line : public Linear {};
		class Ray : public Linear {};
		
		// --- Shape ---

		class Lateral : public Linear {};

		class Diagonal : public Linear {};
		
		class LShape : public NonLinear {};

		// --- Directional Lines (ex: lines not rays. Think math) ---

		class Horizontal : public Lateral, public Line {};

		class Vertical : public Lateral, public Line {};

		// Direction: \ 
		class MainDiagonal : public Diagonal, public Line {};

		// Direction: /
		class OffDiagonal : public Diagonal, public Line {};

		// --- Lateral Rays: Up, Down, Left, Right ---

		class Up : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.upOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false;
			}
		};

		class Down : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.downOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false;
			}
		};

		class Left : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.leftOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.leftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isLeftFile() == false;
			}
		};

		class Right : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.rightOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.rightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isRightFile() == false;
			}
		};

		// --- Diagonal Rays: UL, UR, DL, DR ---

		class UL : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upLeftOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.upLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isLeftFile() == false;
			}
		};

		class UR : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upRightOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.upRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isRightFile() == false;
			}
		};

		class DL : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downLeftOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.downLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isLeftFile() == false;
			}
		};

		class DR : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downRightOne(); }
		
			static BoardSquare move(const BoardSquare & square) {
				return square.downRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isRightFile() == false;
			}
		};

		// --- L-Shaped Directions: Knight Moves --- 
		
		class Knight0 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight0(); }
		};

		class Knight1 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight1(); }
		};
		
		class Knight2 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight2(); }
		};
		
		class Knight3 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight3(); }
		};
		
		class Knight4 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight4(); }
		};
		
		class Knight5 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight5(); }
		};
		
		class Knight6 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight6(); }
		};
		
		class Knight7 : public LShape {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight7(); }
		};

		// --- Reverse Directions ---
		// Primary template
		template<typename DIRECTION_T, typename REVERSE_T>
		inline REVERSE_T reverse();

		// Explicit specializations 
		template<> inline Down reverse<Up>() { return Down{}; }
		template<> inline Up reverse<Down>() { return Up{}; }
		template<> inline Left reverse<Right>() { return Left{}; }
		template<> inline Right reverse<Left>() { return Right{}; }
		template<> inline UL reverse<DR>() { return UL{}; }
		template<> inline DR reverse<UL>() { return DR{}; }
		template<> inline UR reverse<DL>() { return UR{}; }
		template<> inline DL reverse<UR>() { return DL{}; }

	} // namespace directions
} // namespace forge