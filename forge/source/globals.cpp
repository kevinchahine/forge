#include "forge/source/globals.h"

#include <torch/csrc/api/include/torch/cuda.h>

using namespace std;

namespace forge
{
	default_random_engine g_rand(time(nullptr));

	void initGlobals()
	{

	}
} // namespace forge
