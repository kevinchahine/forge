#pragma once

#include "Position.h"

namespace forge
{
	// Type of the heuristics value will be stored in
	// Unit is in centi pawns (1/100 pawn)
	using heuristic_t = int;

	class HeuristicBase
	{
	public:
		// Purpose: Evaluates the favorability of a Chess Position.
		// Positive numbers favor white,
		// Negative numbers favor black,
		// 0 favors a draw.
		// Evaluation is based on the Heursitic class derived from 
		// HeuristicBase and the classes set of weights if it has any.
		virtual heuristic_t eval(const Position & pos) const = 0;

		// Creates a copy of object and returns its address.
		// Equivalent to useing copy assignment opertor on objects and references.
		// For polymorphic base pointers, this can be used to create a copy when 
		// the derived data type of this object is not known. 
		virtual std::unique_ptr<HeuristicBase> clone() const = 0;

		// Creates a copy of object and casts it to the specified data type. 
		// Make sure that the data type of the object matches the specified
		// data type, otherwise errors may occur.
		// If the data type of the object is not known at runtime
		// then use the .clone() method instead.
		template<typename T>
		std::unique_ptr<T> cloneAs() const;

		virtual std::string name() const = 0;

		virtual void print(const Position & pos, std::ostream & os = std::cout) const = 0;

	protected: // --------------------- PROTECTED METHODS ---------------------

		// Counts the the number of a specified piece on the board.
		// White counts as positive, black as negative.
		// That way 3 White Rooks and 1 Black will return (3 - 1 == 2)
		// Or 1 White Rook and 3 Black will return (1 - 3 == -2)
		template<pieces::Piece::piece_t PIECE_VAL>
		int imbalance(const Position & pos) const;

		// Counts how many free spaces a specified piece can move to.
		// Only counts simple moves, enpassent and promotions for pawns and castling for
		// Rooks and Kings. Pieces can only move in ways that they do not put their King
		// in checkmate.
		///template<Piece::piece_t PIECE_VAL>
		///int mobility(const Position & pos) const;
	};

	template<typename T>
	std::unique_ptr<T> HeuristicBase::cloneAs() const
	{
		std::unique_ptr<T> ptr = this->clone();

		return ptr;
	}

	template<pieces::Piece::piece_t PIECE_VAL>
	int HeuristicBase::imbalance(const Position & pos) const
	{
		BitBoard pieces = pos.board().pieces<PIECE_VAL>();	// pieces of some type
		BitBoard whites = pieces & pos.board().whites();	// white pieces of that type
		BitBoard blacks = pieces & pos.board().blacks();	// black pieces of that type

		return 
			static_cast<int>(whites.count()) - 
			static_cast<int>(blacks.count());		// difference between number of each piece
	}
	
	///int HeuristicBase::imbalance<Piece::KING>(const Position & pos) const
	///{
	///	return 0;	// Always will be 0 for Kings. (1 Black King - 1 White King == 0 Kings)
	///}

} // namespace forge