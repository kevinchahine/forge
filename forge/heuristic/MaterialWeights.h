#pragma once

#include "forge/heuristic/Base.h"

namespace forge
{
	namespace heuristic
	{
		class MaterialWeights
		{
		public:
			MaterialWeights() = default;
			MaterialWeights(const MaterialWeights&) = default;
			MaterialWeights(MaterialWeights&&) noexcept = default;
			virtual ~MaterialWeights() noexcept = default;
			MaterialWeights& operator=(const MaterialWeights&) = default;
			MaterialWeights& operator=(MaterialWeights&&) noexcept = default;

			bool operator==(const MaterialWeights& rhs) const {
				return
					// --- Material ---
					queenMaterial == rhs.queenMaterial &&
					rookMaterial == rhs.rookMaterial &&
					bishopMaterial == rhs.bishopMaterial &&
					knightMaterial == rhs.knightMaterial &&
					pawnMaterial == rhs.pawnMaterial &&
					true;
			}

			// --- Material ---
			// Based on value of simply having a number of each piece
			heuristic_t queenMaterial = 901;
			heuristic_t rookMaterial = 501;
			heuristic_t bishopMaterial = 305;
			heuristic_t knightMaterial = 301;
			heuristic_t pawnMaterial = 101;
		}; // class MaterialWeights
	} // namespace heuristic
} // namespace forge