#pragma once

#include <Guten/guten.hpp>

#include <iostream>
#include <thread>
#include <chrono>

namespace forge
{
	namespace test
	{
		namespace feature_extractor
		{
			void init();

			void findAllAttacked();

			void countAllAttacked();

		} // namespace feature_extractor
	} // namespace test
} // namespace forge