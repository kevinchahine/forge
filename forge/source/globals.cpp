#include "globals.h"

using namespace std;

namespace forge
{
	default_random_engine g_rand(time(nullptr));

	//ttttorch::Device g_computingDevice = torch::kCPU;
	
	void initGlobals()
	{
		// --- Computing Device ---
		
		//tttg_computingDevice = torch::kCPU;
		//ttt
		//tttif (torch::cuda::is_available()) {
		//ttt	cout << "--- CUDA is available! ---" << endl
		//ttt		<< torch::cuda::device_count() << " cuda device(s) are available" << endl;
		//ttt
		//ttt	g_computingDevice = torch::kCUDA;
		//ttt}
	}
} // namespace forge
