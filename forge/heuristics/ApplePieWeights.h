#pragma once

#include "forge/heuristics/HeuristicBase.h"

namespace forge
{
	// *** See ApplePieHeuristic
	class ApplePieWeights
	{
	public:
		ApplePieWeights() = default;
		ApplePieWeights(const ApplePieWeights &) = default;
		ApplePieWeights(ApplePieWeights &&) noexcept = default;
		virtual ~ApplePieWeights() noexcept = default;
		ApplePieWeights & operator=(const ApplePieWeights &) = default;
		ApplePieWeights & operator=(ApplePieWeights &&) noexcept = default;

		bool operator==(const ApplePieWeights & rhs) const;

		// --- Material ---
		// Based on value of simply having a number of each piece
		heuristic_t queenMaterial = 901;
		heuristic_t rookMaterial = 501;
		heuristic_t bishopMaterial = 305;
		heuristic_t knightMaterial = 301;
		heuristic_t pawnMaterial = 101;
	}; // class ApplePieWeights
} // namespace forge