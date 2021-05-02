#pragma once

#include "WeightsBase.h"
#include "ApplePieHeuristic.h"

namespace forge
{
	// *** See ApplePieHeuristic
	class ApplePieWeights : public WeightsBase
	{
	public:
		ApplePieWeights() = default;
		ApplePieWeights(const ApplePieWeights &) = default;
		ApplePieWeights(ApplePieWeights &&) noexcept = default;
		virtual ~ApplePieWeights() noexcept = default;
		ApplePieWeights & operator=(const ApplePieWeights &) = default;
		ApplePieWeights & operator=(ApplePieWeights &&) noexcept = default;

		virtual void serialize(WeightsArchive & ar) const override;
		virtual void parse(WeightsArchive & ar) override;

		// --- Material ---
		heuristic_t queenWeight = 0;
		heuristic_t rookWeight = 0;
		heuristic_t bishopWeight = 0;
		heuristic_t knightWeight = 0;
		heuristic_t pawnWeight = 0;
		heuristic_t bishopPair = 0;
		heuristic_t oppositeBishop = 0;
		
		// --- Mobility ---
		heuristic_t queenMobility = 0;
		heuristic_t rookMobility = 0;
		heuristic_t bishopMobility = 0;
		heuristic_t knightMobility = 0;
		heuristic_t pawnMobility = 0;
		heuristic_t kingMobility = 0;
		
		// --- Piece Square Table Bonus  = 0;
		heuristic_t queenPSTB = 0;
		heuristic_t rookPSTB = 0;
		heuristic_t bishopPSTB = 0;
		heuristic_t knightPSTB = 0;
		heuristic_t pawnPSTB = 0;
		heuristic_t kingPSTB = 0;
		
		// --- Defended Pieces ---
		heuristic_t queensDefended = 0;
		heuristic_t rooksDefended = 0;
		heuristic_t bishopsDefended = 0;
		heuristic_t knightsDefended = 0;
		heuristic_t pawnsDefended = 0;
		heuristic_t kingsDefended = 0;

		// --- Attacking Pieces ---
	}; // class ApplePieWeights
} // namespace forge