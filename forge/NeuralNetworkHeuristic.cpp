#include "NeuralNetworkHeuristic.h"

using namespace std;

namespace forge
{
	NeuralNetworkHeuristic::NeuralNetworkHeuristic() :
		m_model(OpenNN::NeuralNetwork(OpenNN::NeuralNetwork::ProjectType::Approximation, {832, 1000, 500, 1}))
	{}
	
	//NeuralNetworkHeuristic::NeuralNetworkHeuristic(const std::string& model_file_name)
	//{
	//	cout << __FUNCTION__ << " not implemented\n";
	//	//m_model = ml::Optimizer::createNN();
	//}

	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		size_t inputSize = m_model.get_layer_pointer(0)->get_inputs_number();

		Eigen::Tensor<float, 2> inputs(1, inputSize);	// TODO: do this in 2D. Let 1st dim be the feature and the 2nd be the square
		Eigen::Tensor<float, 2> outputs(1, 1);

		// --- Preprocess ---

		// --- Feature Extraction ---
		inputs = this->featureExtraction(pos);

		// --- Feed Forward ---
		outputs = m_model.calculate_outputs(inputs);

		// --- Return ---
		return outputs(0, 0);
	}

	unique_ptr<HeuristicBase> NeuralNetworkHeuristic::clone() const
	{
		return make_unique<NeuralNetworkHeuristic>(*this);
	}

	void NeuralNetworkHeuristic::print(const Position& pos, std::ostream& os) const
	{
		cout << __FUNCTION__ << " not fully implemented" << endl;
	}

	void NeuralNetworkHeuristic::train()
	{	
		// --- Load Training Data ---

		// --- Preprocess Data ---

		int nSamples = 10;
		int nFeatures = 832;
		//Eigen::Tensor<float, 2> data(nFeatures, nSamples);
		// OpenNN::DataSet dataSet(data);
		
		OpenNN::DataSet dataSet(nSamples, nFeatures, 1);
		cout << "samples number = " << dataSet.get_samples_number() << endl
			<< "columns number: " << dataSet.get_columns_number() << endl
			<< "columns input number: " << dataSet.get_input_columns_number() << endl
			<< "columns target number: " << dataSet.get_target_columns_number() << endl
			<< endl;

		Eigen::Tensor<float, 2> data = dataSet.get_data();	// !!! Does not return a non-const reference

		cout << "type = " << typeid(data).name() << endl
			<< "rank = " << data.rank() << endl
			<< "size = " << data.size() << endl
			<< "dimention[0] = " << data.dimension(0) << endl
			<< "dimention[1] = " << data.dimension(1) << endl
			<< endl;

		for (size_t s = 0; s < nSamples; s++) {
			Position pos;
			Eigen::Tensor<float, 2> sample = this->featureExtraction(pos);

			for (size_t col = 0; col < data.dimension(1); col++) {
				data(s, col) = sample(0, col);
			}
		}

		OpenNN::TrainingStrategy trainingStrategy(&m_model, &dataSet);
		trainingStrategy.set_loss_method(OpenNN::TrainingStrategy::LossMethod::MEAN_SQUARED_ERROR);
		trainingStrategy.set_optimization_method(OpenNN::TrainingStrategy::OptimizationMethod::GRADIENT_DESCENT);
		
		trainingStrategy.perform_training();
	}

	Eigen::Tensor<float, 2> NeuralNetworkHeuristic::featureExtraction(const Position & pos) 
	{
		// --- Alias ---
		Board b = (pos.moveCounter().isWhitesTurn() ? pos.board() : pos.board().rotated());

		BitBoard ours = b.whites();
		BitBoard theirs = b.blacks();

		BitBoard ourKings = ours & b.kings();
		BitBoard ourQueens = ours & b.queens();
		BitBoard ourBishops = ours & b.bishops();
		BitBoard ourKnights = ours & b.knights();
		BitBoard ourRooks = ours & b.rooks();
		BitBoard ourPawns = ours & b.pawns();

		BitBoard theirKings = theirs & b.kings();
		BitBoard theirQueens = theirs & b.queens();
		BitBoard theirBishops = theirs & b.bishops();
		BitBoard theirKnights = theirs & b.knights();
		BitBoard theirRooks = theirs & b.rooks();
		BitBoard theirPawns = theirs & b.pawns();

		Eigen::Tensor<float, 2> inputs(1, this->nInputNodes());
		
		for (size_t bit = 0; bit < 64; bit++) {
			// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
			inputs(1, 64 * 0 + bit) = ourKings[bit];
			inputs(1, 64 * 1 + bit) = ourQueens[bit];
			inputs(1, 64 * 2 + bit) = ourBishops[bit];
			inputs(1, 64 * 3 + bit) = ourKnights[bit];
			inputs(1, 64 * 4 + bit) = ourRooks[bit];
			inputs(1, 64 * 5 + bit) = ourPawns[bit];
			inputs(1, 64 * 6 + bit) = theirKings[bit];
			inputs(1, 64 * 7 + bit) = theirQueens[bit];
			inputs(1, 64 * 8 + bit) = theirBishops[bit];
			inputs(1, 64 * 9 + bit) = theirKnights[bit];
			inputs(1, 64 * 10 + bit) = theirRooks[bit];
			inputs(1, 64 * 11 + bit) = theirPawns[bit];
		}

		return inputs;
	}

} // namespace forge