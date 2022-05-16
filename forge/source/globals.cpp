#include "forge/source/globals.h"

using namespace std;

namespace forge
{
	default_random_engine g_rand(time(nullptr));

	torch::Device g_computingDevice = torch::kCPU;
	
	void initGlobals()
	{
		// --- Computing Device ---
		
		g_computingDevice = torch::kCPU;
		
		if (torch::cuda::is_available()) {
			cout << "--- CUDA is available! ---" << endl
				<< torch::cuda::device_count() << " cuda device(s) are available" << endl;
		
			g_computingDevice = torch::kCUDA;
		}
	}
} // namespace forge
