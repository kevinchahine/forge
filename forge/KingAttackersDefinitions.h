#pragma once

#include "KingAttackers.h"

namespace forge
{
	template<typename RAY_DIRECTION_T>
	inline void KingAttackers::push_back_rays(BoardSquare king, BoardSquare attacker)
	{
		static_assert(std::is_base_of<directions::Ray, RAY_DIRECTION_T>(),
			"RAY_DIRECTION_T must be of a Ray direction");

		BitBoard lineOfFire = BitBoard::mask<RAY_DIRECTION_T>(king, attacker);
		lineOfFire[attacker] = 1;

		// Call push_back from base class
		this->super::push_back(
			KingAttacker{
				attacker,
				lineOfFire,
				RAY_DIRECTION_T{}
			});
	}

	template<typename NON_RAY_DIRECTION>
	inline void KingAttackers::push_back_non_rays(BoardSquare attacker)
	{
		//static_assert(std::is_base_of<directions::Ray, NON_RAY_DIRECTION>() == false,
		//	"NON_RAY_DIRECTION must be of a Non-Ray direction");

		this->super::push_back(
			KingAttacker{
				attacker,
				BitBoard{},
				NON_RAY_DIRECTION{}
			}
		);
	}
} // namespace forge