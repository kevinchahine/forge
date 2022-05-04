#include "TensorPair.h"

#include "../globals.h"

using namespace std;

namespace forge
{
	TensorPair::TensorPair(int64_t nSamples, int64_t nInputFeatures, int64_t nOutputFeatures) :
		TensorPair(nSamples, nInputFeatures, nOutputFeatures, g_computingDevice)
	{
	}

	TensorPair::TensorPair(int64_t nSamples, int64_t nInputFeatures, int64_t nOutputFeatures, const torch::Device & computingDevice) :
		inputs(torch::zeros({ nSamples, nInputFeatures }, computingDevice)),
		outputs(torch::zeros({ nSamples, nOutputFeatures }, computingDevice))
	{
	}

	void TensorPair::moveTo(const torch::Device& device)
	{
		inputs = inputs.to(device);
		outputs = outputs.to(device);
	}

	ostream& operator<<(ostream& os, const TensorPair& pair)
	{
		os << "Inputs: " << endl
			<< pair.inputs << endl
			<< "Outputs: " << endl
			<< pair.outputs << endl;

		return os;
	}
} // namespace forge