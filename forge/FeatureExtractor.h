#pragma once

#include "Position.h"

#include <iostream>

#include "opennn/opennn.h"		// TODO: all we need is Eigen::Tensor

namespace forge
{
	class FeatureExtractor
	{
	public:
		void init(const Position & pos);

		Eigen::Tensor<float, 2> extractMaterial();
		Eigen::Tensor<float, 2> extractMobility();
		// ... Insert more featues here ...

	protected:
		BitBoard ours;
		BitBoard theirs;

		BitBoard ourKings;
		BitBoard ourQueens;
		BitBoard ourBishops;
		BitBoard ourKnights;
		BitBoard ourRooks;
		BitBoard ourPawns;

		BitBoard theirKings;
		BitBoard theirQueens;
		BitBoard theirBishops;
		BitBoard theirKnights;
		BitBoard theirRooks;
		BitBoard theirPawns;
	};
} // namespace forge
