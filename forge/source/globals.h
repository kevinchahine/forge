#pragma once

#include <random>
#include <chrono>
#include <memory>

#include <torch/csrc/api/include/torch/serialize.h>

// --- Global Variables ---
namespace forge
{
	extern std::default_random_engine g_rand;

	// computing device to be used by tensor and neural network opterations
	extern torch::Device g_computingDevice;

	void initGlobals();
} // namespace forge