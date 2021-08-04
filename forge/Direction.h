#pragma once

#include "BoardSquare.h"

namespace forge
{
	namespace directions
	{
		class Direction {
		public:
			Direction() = default;
			Direction(int8_t vertical, int8_t horizontal) :
				vertical(vertical),
				horizontal(horizontal) {}
			Direction(const Direction &) = default;
			Direction(Direction &&) noexcept = default;
			~Direction() noexcept = default;
			Direction & operator=(const Direction &) = default;
			Direction & operator=(Direction &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) {
				return BoardSquare{ bs.row() + vertical, bs.col() + horizontal };
			}

			BoardSquare move(const BoardSquare & square) const {
				return BoardSquare{ square.row() + vertical, square.col() + horizontal };
			}

			BoardSquare reverse(const BoardSquare & square) const {
				return BoardSquare{ square.row() - vertical, square.col() - horizontal };
			}

			bool wouldBeInBounds(const BoardSquare & square) const {
				return
					static_cast<int8_t>(square.row()) + vertical < 8 &&
					static_cast<int8_t>(square.col()) + horizontal < 8;
			}

			//bool isLateral() const { return vertical }
			bool isDiagonal() const { return abs(vertical) == abs(horizontal); }
			bool isVertical() const { return vertical != 0 && horizontal == 0; }
			bool isHorizontal() const { return vertical == 0 && horizontal != 0; }

			static std::string str() { return "Direction"; }

		public:
			int8_t vertical = 0;
			int8_t horizontal = 0;
		};

		// --- Line Directions (Mixin Class)  ---

		class Linear : public virtual Direction {};
		class NonLinear : public virtual Direction {};
		class Line : public virtual Linear {};
		class Ray : public virtual Linear {};

		// --- Shape ---

		class Lateral : public virtual Linear {};

		class Diagonal : public virtual Linear {};

		class LShape : public virtual NonLinear {};

		// --- Directional Lines (ex: lines not rays. Think math) ---

		class Horizontal : public virtual Lateral, public virtual Line {};

		class Vertical : public virtual Lateral, public virtual Line {};

		// Direction: \ 
		class MainDiagonal : public virtual Diagonal, public virtual Line {};

		// Direction: /
		class OffDiagonal : public virtual Diagonal, public virtual Line {};

		// --- Lateral Rays: Up, Down, Left, Right ---

		class Up : public virtual Lateral, virtual public Ray {
		public:
			Up() : Direction{ -1, 0 } {}
			Up(const Up &) = default;
			Up(Up &&) noexcept = default;
			~Up() noexcept = default;
			Up & operator=(const Up &) = default;
			Up & operator=(Up &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.upOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.upOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.downOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false;
			}

			static std::string str() { return "Up"; }
		};

		class Down : public Lateral, public Ray {
		public:
			Down() : Direction{ 1, 0 } {}
			Down(const Down &) = default;
			Down(Down &&) noexcept = default;
			~Down() noexcept = default;
			Down & operator=(const Down &) = default;
			Down & operator=(Down &&) noexcept = default;
			
			BoardSquare operator()(BoardSquare bs) { return bs.downOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.downOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.upOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false;
			}

			static std::string str() { return "Down"; }
		};

		class Left : public Lateral, public Ray {
		public:
			Left() : Direction{ 0, -1 } {}
			Left(const Left &) = default;
			Left(Left &&) noexcept = default;
			~Left() noexcept = default;
			Left & operator=(const Left &) = default;
			Left & operator=(Left &&) noexcept = default;
			
			BoardSquare operator()(BoardSquare bs) { return bs.leftOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.leftOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.rightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isLeftFile() == false;
			}

			static std::string str() { return "Left"; }
		};

		class Right : public Lateral, public Ray {
		public:
			Right() : Direction{ 0, 1 } {}
			Right(const Right &) = default;
			Right(Right &&) noexcept = default;
			~Right() noexcept = default;
			Right & operator=(const Right &) = default;
			Right & operator=(Right &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.rightOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.rightOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.leftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isRightFile() == false;
			}

			static std::string str() { return "Right"; }
		};

		// --- Diagonal Rays: UL, UR, DL, DR ---

		class UL : public Diagonal, public Ray {
		public:
			UL() : Direction{ -1, -1 } {}
			UL(const UL &) = default;
			UL(UL &&) noexcept = default;
			~UL() noexcept = default;
			UL & operator=(const UL &) = default;
			UL & operator=(UL &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.upLeftOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.upLeftOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.downRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isLeftFile() == false;
			}

			static std::string str() { return "UL"; }
		};

		class UR : public Diagonal, public Ray {
		public:
			UR() : Direction{ -1, 1 } {}
			UR(const UR &) = default;
			UR(UR &&) noexcept = default;
			~UR() noexcept = default;
			UR & operator=(const UR &) = default;
			UR & operator=(UR &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.upRightOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.upRightOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.downLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isTopRank() == false && square.isRightFile() == false;
			}

			static std::string str() { return "UR"; }
		};

		class DL : public Diagonal, public Ray {
		public:
			DL() : Direction{ 1, -1 } {}
			DL(const DL &) = default;
			DL(DL &&) noexcept = default;
			~DL() noexcept = default;
			DL & operator=(const DL &) = default;
			DL & operator=(DL &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.downLeftOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.downLeftOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.upRightOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isLeftFile() == false;
			}

			static std::string str() { return "DL"; }
		};

		class DR : public Diagonal, public Ray {
		public:
			DR() : Direction{ 1, 1 } {}
			DR(const DR &) = default;
			DR(DR &&) noexcept = default;
			~DR() noexcept = default;
			DR & operator=(const DR &) = default;
			DR & operator=(DR &&) noexcept = default;

			BoardSquare operator()(BoardSquare bs) { return bs.downRightOne(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.downRightOne();
			}

			static BoardSquare reverse(const BoardSquare & square) {
				return square.upLeftOne();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isBotRank() == false && square.isRightFile() == false;
			}

			static std::string str() { return "DR"; }
		};

		// --- L-Shaped Directions: Knight Moves --- 

		class Knight0 : public LShape {
		public:	
			Knight0() : Direction{ -1, 2 } {}
			Knight0(const Knight0 &) = default;
			Knight0(Knight0 &&) noexcept = default;
			~Knight0() noexcept = default;
			Knight0 & operator=(const Knight0 &) = default;
			Knight0 & operator=(Knight0 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight0(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight0();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight0InBounds();
			}

			static std::string str() { return "Knight0"; }
		};

		class Knight1 : public LShape {
		public:	
			Knight1() : Direction{ -2, 1 } {}
			Knight1(const Knight1 &) = default;
			Knight1(Knight1 &&) noexcept = default;
			~Knight1() noexcept = default;
			Knight1 & operator=(const Knight1 &) = default;
			Knight1 & operator=(Knight1 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight1(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight1();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight1InBounds();
			}

			static std::string str() { return "Knight1"; }
		};

		class Knight2 : public LShape {
		public:	
			Knight2() : Direction{ -2, -1 } {}
			Knight2(const Knight2 &) = default;
			Knight2(Knight2 &&) noexcept = default;
			~Knight2() noexcept = default;
			Knight2 & operator=(const Knight2 &) = default;
			Knight2 & operator=(Knight2 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight2(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight2();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight2InBounds();
			}

			static std::string str() { return "Knight2"; }
		};

		class Knight3 : public LShape {
		public:	
			Knight3() : Direction{ -1, -2 } {}
			Knight3(const Knight3 &) = default;
			Knight3(Knight3 &&) noexcept = default;
			~Knight3() noexcept = default;
			Knight3 & operator=(const Knight3 &) = default;
			Knight3 & operator=(Knight3 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight3(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight3();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight3InBounds();
			}

			static std::string str() { return "Knight3"; }
		};

		class Knight4 : public LShape {
		public:	
			Knight4() : Direction{ 1, -2 } {}
			Knight4(const Knight4 &) = default;
			Knight4(Knight4 &&) noexcept = default;
			~Knight4() noexcept = default;
			Knight4 & operator=(const Knight4 &) = default;
			Knight4 & operator=(Knight4 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight4(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight4();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight4InBounds();
			}

			static std::string str() { return "Knight4"; }
		};

		class Knight5 : public LShape {
		public:	
			Knight5() : Direction{ 2, -1 } {}
			Knight5(const Knight5 &) = default;
			Knight5(Knight5 &&) noexcept = default;
			~Knight5() noexcept = default;
			Knight5 & operator=(const Knight5 &) = default;
			Knight5 & operator=(Knight5 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight5(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight5();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight5InBounds();
			}

			static std::string str() { return "Knight5"; }
		};

		class Knight6 : public LShape {
		public:	
			Knight6() : Direction{ 2, 1 } {}
			Knight6(const Knight6 &) = default;
			Knight6(Knight6 &&) noexcept = default;
			~Knight6() noexcept = default;
			Knight6 & operator=(const Knight6 &) = default;
			Knight6 & operator=(Knight6 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight6(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight6();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight6InBounds();
			}

			static std::string str() { return "Knight6"; }
		};

		class Knight7 : public LShape {
		public:	
			Knight7() : Direction{ 1, 2 } {}
			Knight7(const Knight7 &) = default;
			Knight7(Knight7 &&) noexcept = default;
			~Knight7() noexcept = default;
			Knight7 & operator=(const Knight7 &) = default;
			Knight7 & operator=(Knight7 &&) noexcept = default;
			
		public:
			BoardSquare operator()(BoardSquare bs) { return bs.knight7(); }

			static BoardSquare move(const BoardSquare & square) {
				return square.knight7();
			}

			static bool wouldBeInBounds(const BoardSquare & square) {
				return square.isKnight7InBounds();
			}

			static std::string str() { return "Knight7"; }
		};

		// --- Checks if two BoardSquares are in some direction ---

		///template<typename DIRECTION_T>
		///bool isPointingTo(BoardSquare from, BoardSquare to);
		///template<> inline bool isPointingTo<Up>(BoardSquare from, BoardSquare to)
		///{
		///	return from.col() == to.col() && from.row() > to.row();
		///}
		///template<> inline bool isPointingTo<Down>(BoardSquare from, BoardSquare to)
		///{
		///	return from.col() == to.col() && from.row() < to.row();
		///}
		///template<> inline bool isPointingTo<Left>(BoardSquare from, BoardSquare to)
		///{
		///	return from.col() > to.col() && from.row() == to.row();
		///}
		///template<> inline bool isPointingTo<Right>(BoardSquare from, BoardSquare to)
		///{
		///	return from.col() < to.col() && from.row() == to.row();
		///}
		///template<> inline bool isPointingTo<UR>(BoardSquare from, BoardSquare to)
		///{
		///	uint8_t deltaRow = from.row() - to.row();
		///	uint8_t deltaCol = to.col() - from.col();
		///
		///	return deltaRow == deltaCol && deltaRow < 8 && deltaCol < 8;
		///}
		///template<> inline bool isPointingTo<UL>(BoardSquare from, BoardSquare to)
		///{
		///	uint8_t deltaRow = from.row() - to.row();
		///	uint8_t deltaCol = from.col() - to.col();
		///
		///	return deltaRow == deltaCol && deltaRow < 8 && deltaCol < 8;
		///}
		///template<> inline bool isPointingTo<DL>(BoardSquare from, BoardSquare to)
		///{
		///	uint8_t deltaRow = to.row() - from.row();
		///	uint8_t deltaCol = from.col() - to.col();
		///
		///	return deltaRow == deltaCol && deltaRow < 8 && deltaCol < 8;
		///}
		///template<> inline bool isPointingTo<DR>(BoardSquare from, BoardSquare to)
		///{
		///	uint8_t deltaRow = to.row() - from.row();
		///	uint8_t deltaCol = to.col() - from.col();
		///
		///	return deltaRow == deltaCol && deltaRow < 8 && deltaCol < 8;
		///}

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