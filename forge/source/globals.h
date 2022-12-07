#pragma once

#include <random>
#include <chrono>
#include <memory>

// --- Global Variables ---
namespace forge
{
	extern std::default_random_engine g_rand;

	void initGlobals();
} // namespace forge