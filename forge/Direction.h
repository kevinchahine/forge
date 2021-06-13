#pragma once

#include "BoardSquare.h"

namespace forge
{
	namespace directions
	{
		class Direction {};

		// --- Shape ---

		class Lateral : public Direction {};

		class Diagonal : public Direction {};
		
		class LShape : public Direction {};

		// --- Line Directions (Mixin Class)  ---

		class Line {};

		// --- Directional Lines (ex: lines not rays. Think math) ---

		class Horizontal : public Lateral, public Line {};

		class Vertical : public Lateral, public Line {};

		// Direction: \ 
		class MainDiagonal : public Diagonal, public Line {};

		// Direction: /
		class OffDiagonal : public Diagonal, public Line {};

		// --- Rays (Mixin Class) ---

		class Ray : public Direction {};

		// --- Lateral Rays: Up, Down, Left, Right ---

		class Up : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upOne(); }
		};

		class Down : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downOne(); }
		};

		class Left : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.leftOne(); }
		};

		class Right : public Lateral, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.rightOne(); }
		};

		// --- Diagonal Rays: UL, UR, DL, DR ---

		class UL : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upLeftOne(); }
		};

		class UR : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.upRightOne(); }
		};

		class DL : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downLeftOne(); }
		};

		class DR : public Diagonal, public Ray {
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.downRightOne(); }
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
	} // namespace directions
} // namespace forge