#include "forge/source/globals_torch.h"

#include <torch/csrc/api/include/torch/cuda.h>

using namespace std;

namespace forge
{
	torch::Device g_computingDevice = torch::kCPU;

	std::filesystem::path g_checkpointDir = CHECKPOINT_DIR;
	
	void initGlobalsTorch()
	{
		// --- Computing Device ---

		g_computingDevice = torch::kCPU;

		if (torch::cuda::is_available()) {
			cout << "--- CUDA is available! ---" << endl
				<< torch::cuda::device_count() << " cuda device(s) are available" << endl;

			g_computingDevice = torch::kCUDA;
		}
		else {
			cout << "--- CUDA is NOT available! ---" << endl
				<< "Using CPU for tensor operations instead" << endl;
		}

		cout << "Checkpoint Dir = " << forge::g_checkpointDir << endl;
	}
} // namespace forge
