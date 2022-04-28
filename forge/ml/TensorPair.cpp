#include "TensorPair.h"

using namespace std;

namespace forge
{
	TensorPair::TensorPair(int64_t nSamples, int64_t nInputFeatures, int64_t nOutputFeatures) :
		inputs(torch::zeros({ nSamples, nInputFeatures })),
		outputs(torch::zeros({ nSamples, nOutputFeatures }))
	{}

	ostream& operator<<(ostream& os, const TensorPair& pair)
	{
		os << "Inputs: " << endl
			<< pair.inputs << endl
			<< "Outputs: " << endl
			<< pair.outputs << endl;

		return os;
	}
} // namespace forge