#pragma once

#include "Position.h"

#include <iostream>

#include <torch/csrc/api/include/torch/data/transforms/tensor.h>

namespace forge
{
	class FeatureExtractor
	{
	public:
		void init(const Position & pos);

		torch::Tensor extractMaterial();
		torch::Tensor extractMobility();
		// ... Insert more featues here ...

	public:
		static const size_t MATERIAL_FEATURES_SIZE = 13 * 64;
		static const size_t MOBILITY_FEATURES_SIZE = 12 * 64;

	protected:
		Board b;
		
		BitBoard ours;
		BitBoard theirs;
		BitBoard empty;

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
