#pragma once 

#include <torch/csrc/api/include/torch/serialize.h>

// --- Global Variables ---
namespace forge
{
	// computing device to be used by tensor and neural network opterations
	extern torch::Device g_computingDevice;

	void initGlobalsTorch();
} // namespace forge