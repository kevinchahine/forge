#pragma once

#include "WeightsBase.h"
#include "ApplePieHeuristic.h"

namespace forge
{
	// *** See ApplePieHeuristic
	class ApplePieWeights : public WeightsBase
	{
	public:
		ApplePieWeights() : WeightsBase{ 100 } {}	// TODO: Just For now
		ApplePieWeights(const ApplePieWeights &) = default;
		ApplePieWeights(ApplePieWeights &&) noexcept = default;
		virtual ~ApplePieWeights() noexcept = default;
		ApplePieWeights & operator=(const ApplePieWeights &) = default;
		ApplePieWeights & operator=(ApplePieWeights &&) noexcept = default;

		///// --- Material ---
		///heuristic_t queenWeight() const { return (*this)[0]; }
		///heuristic_t rookWeight() const { return (*this)[1]; }
		///heuristic_t bishopWeight() const { return (*this)[2]; }
		///heuristic_t knightWeight() const { return (*this)[3]; }
		///heuristic_t pawnWeight() const { return (*this)[4]; }
		///heuristic_t bishopPair() const { return (*this)[5]; }
		///heuristic_t oppositeBishop() const { return (*this)[6]; }
		///
		///// --- Mobility ---
		///heuristic_t queenMobility() const { return (*this)[7]; }
		///heuristic_t rookMobility() const { return (*this)[8]; }
		///heuristic_t bishopMobility() const { return (*this)[9]; }
		///heuristic_t knightMobility() const { return (*this)[10]; }
		///heuristic_t pawnMobility() const { return (*this)[11]; }
		///heuristic_t kingMobility() const { return (*this)[12]; }
		///
		///// --- Piece Square Table Bonus (pstb) ---
		///heuristic_t queenPSTB(BoardSquare cell) const { return (*this)[13 + cell.val()]; }
		///heuristic_t rookPSTB(BoardSquare cell) const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t bishopPSTB(BoardSquare cell) const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t knightPSTB(BoardSquare cell) const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t pawnPSTB(BoardSquare cell) const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t kingPSTB(BoardSquare cell) const { return (*this)[13 + 64 + cell.val()]; }
		///
		///// --- Defended Pieces ---
		///heuristic_t queensDefended() const { return (*this)[13 + cell.val()]; }
		///heuristic_t rooksDefended() const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t bishopsDefended() const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t knightsDefended() const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t pawnsDefended() const { return (*this)[13 + 64 + cell.val()]; }
		///heuristic_t kingsDefended() const { return (*this)[13 + 64 + cell.val()]; }

		// --- Attacking Pieces ---
	}; // class ApplePieWeights
} // namespace forge