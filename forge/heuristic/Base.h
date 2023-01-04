#pragma once

#include <vector>

#include <forge/core/Position.h>

namespace forge
{
	// Type of the heuristics value will be stored in
	// Unit is in centi pawns (1/100 pawn)
	using heuristic_t = int;

	namespace heuristic {
		class Base
		{
		public:
			// Purpose: Evaluates the favorability of a Chess Position.
			// Positive numbers favor white,
			// Negative numbers favor black,
			// 0 favors a draw.
			// Evaluation is based on the Heursitic class derived from 
			// Base and the classes set of weights if it has any.
			// !Warning!: Evaluation does not consider game state.
			// Ex: If a position is a mate, evaluation may give an evaluation
			//	that indicates something else, like a draw.
			virtual heuristic_t eval(const Position& pos) = 0;

			// Purpose: Evaluates the favorability of a Chess Position from the 
			// perspective of the player running the search ("The thinking player").
			// This player is specified by `whiteIsSearching`.
			// If `whiteIsSearching` is true, then white player is thinking and positive evaluations will favor the white player.
			// If `whiteIsSearching` is false, then black player is thinking and positive evaluations will favor the black player.
			// !Warning!: Evaluation does not consider game state.
			// Ex: If a position is a mate, evaluation may give an evaluation
			//	that indicates something else, like a draw.
			virtual heuristic_t eval(const Position& pos, bool whiteIsSearching) = 0;

			// TODO: make pure virtual
			virtual std::vector<heuristic_t> eval(const std::vector<const Position *>& positions, bool whiteIsSearching) = 0;

			// Creates a copy of object and returns its address.
			// Equivalent to useing copy assignment opertor on objects and references.
			// For polymorphic base pointers, this can be used to create a copy when 
			// the derived data type of this object is not known. 
			virtual std::unique_ptr<Base> clone() const = 0;

			// Creates a copy of object and casts it to the specified data type. 
			// Make sure that the data type of the object matches the specified
			// data type, otherwise errors may occur.
			// If the data type of the object is not known at runtime
			// then use the .clone() method instead.
			template<typename T>
			std::unique_ptr<T> cloneAs() const;

			virtual std::string name() const = 0;

			virtual void print(const Position& pos, std::ostream& os = std::cout) const = 0;

		protected: // --------------------- PROTECTED METHODS ---------------------

			// Counts the the number of a specified piece on the board.
			// White counts as positive, black as negative.
			// That way 3 White Rooks and 1 Black will return (3 - 1 == 2)
			// Or 1 White Rook and 3 Black will return (1 - 3 == -2)
			template<typename PIECE_T>
			int imbalance(const Position& pos) const;

			// Counts how many free spaces a specified piece can move to.
			// Only counts simple moves, enpassent and promotions for pawns and castling for
			// Rooks and Kings. Pieces can only move in ways that they do not put their King
			// in checkmate.
			///template<Piece::piece_t PIECE_VAL>
			///int mobility(const Position & pos) const;
		}; // class Base

		template<typename T>
		std::unique_ptr<T> Base::cloneAs() const
		{
			std::unique_ptr<T> ptr = this->clone();

			return ptr;
		}

		template<typename PIECE_T>
		int Base::imbalance(const Position& pos) const
		{
			BitBoard pieces = pos.board().pieces<PIECE_T>();	// pieces of some type
			BitBoard whites = pieces & pos.board().whites();	// white pieces of that type
			BitBoard blacks = pieces & pos.board().blacks();	// black pieces of that type

			return
				static_cast<int>(whites.count()) -
				static_cast<int>(blacks.count());		// difference between number of each piece
		}
	} // namespace heuristic
} // namespace forge