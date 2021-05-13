#pragma once

#include "WeightsBase.h"
#include "PositionTable.h"

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

		bool operator==(const ApplePieWeights & rhs) const;

		virtual void serialize(WeightsArchive & ar) const override;
		virtual void parse(WeightsArchive & ar) override;

		// --- Material ---
		// Based on value of simply having a number of each piece
		heuristic_t queenMaterial = 901;
		heuristic_t rookMaterial = 501;
		heuristic_t bishopMaterial = 305;
		heuristic_t knightMaterial = 301;
		heuristic_t pawnMaterial = 101;

		// --- 
		heuristic_t bishopPair = 0;
		heuristic_t oppositeBishop = 0;
		
		// --- Mobility ---
		// Based on how many spaces each piece can move.
		heuristic_t queenMobility = 0;
		heuristic_t rookMobility = 0;
		heuristic_t bishopMobility = 0;
		heuristic_t knightMobility = 0;
		heuristic_t pawnMobility = 0;
		heuristic_t kingMobility = 0;
		
		// --- Piece Square Table Bonus ---
		PositionTable queenPSTB;
		PositionTable rookPSTB;
		PositionTable bishopPSTB;
		PositionTable knightPSTB;
		PositionTable pawnPSTB;
		PositionTable kingPSTB;
		
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