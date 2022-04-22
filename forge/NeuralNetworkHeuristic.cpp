#include "NeuralNetworkHeuristic.h"

using namespace std;

namespace forge
{
	NeuralNetworkHeuristic::NeuralNetworkHeuristic()// :
		//m_model(/*OpenNN::NeuralNetwork(OpenNN::NeuralNetwork::ProjectType::Approximation, {forge::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1000, 500, 1})*/)
	{	}
	
	//NeuralNetworkHeuristic::NeuralNetworkHeuristic(const std::string& model_file_name)
	//{
	//	cout << __FUNCTION__ << " not implemented\n";
	//	//m_model = ml::Optimizer::createNN();
	//}

	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		////size_t inputSize = m_model.get_layer_pointer(0)->get_inputs_number();

		////Eigen::Tensor<float, 2> inputs(1, inputSize);	// TODO: do this in 2D. Let 1st dim be the feature and the 2nd be the square
		////Eigen::Tensor<float, 2> outputs(1, 1);

		// --- Preprocess ---

		// --- Feature Extraction ---
		////inputs = this->featureExtraction(pos);

		// --- Feed Forward ---
		////outputs = m_model.calculate_outputs(inputs);

		// --- Return ---
		return 0.0;//// outputs(0, 0);
	}

	unique_ptr<HeuristicBase> NeuralNetworkHeuristic::clone() const
	{
		return make_unique<NeuralNetworkHeuristic>(*this);
	}

	void NeuralNetworkHeuristic::print(const Position& pos, std::ostream& os) const
	{
		cout << __FUNCTION__ << " not fully implemented" << endl;
	}

	void NeuralNetworkHeuristic::train(DataSet & trainingDS)
	{	
		int counter = 0;

		// Repeat until we run out of data
		while (true) {
			trainingDS.generateNextBatch();
			break;	// TODO: remove this
			////// Break when we train on our last batch of samples
			////if (trainingDS.get_samples_number() == 0) {
			////	cout << "Last batch of data was reached" << endl;
			////	break;
			////}
			////
			////OpenNN::TrainingStrategy trainingStrategy(&m_model, &trainingDS);
			////
			//////trainingStrategy.set_display_period(50);	// show progress every 50 epochs
			////trainingStrategy.set_maximum_epochs_number(50); //200);
			////trainingStrategy.set_loss_method(OpenNN::TrainingStrategy::LossMethod::MEAN_SQUARED_ERROR);
			////trainingStrategy.set_optimization_method(
			////	//OpenNN::TrainingStrategy::OptimizationMethod::GRADIENT_DESCENT);
			////	OpenNN::TrainingStrategy::OptimizationMethod::ADAPTIVE_MOMENT_ESTIMATION);
			////
			////trainingStrategy.perform_training();
			////
			////// Save training progress
			////// TODO: There is a more standardized way to do this
			////// TODO: Make sure directory exists before saving
			//////stringstream ss;
			//////ss << "trained_models/trained_nn_" << counter++ << ".xml";
			//////m_model.save(ss.str());
			////
			////const Eigen::Tensor<float, 2> & ds = trainingDS.get_data();
			////
			////cout << "--- Evaluate ---" << endl;
			//////for (int i = 0; i < 5; i++) {
			//////	Eigen::Tensor<float, 2> inputs(1, ds.dimension(1));
			//////
			//////	for (int col = 0; col < inputs.dimension(1); col++) {
			//////		inputs(0, col) = inputs(0, col);
			//////	}
			//////
			//////	Eigen::Tensor<float, 2> guess = m_model.calculate_outputs(inputs);
			//////
			//////	cout << "Guess " << i << ": " << guess(0, 0) << endl;
			//////}
			////// Break when we meet the stopping criteria
			////// TODO: something goes here
		}

		// Save model
		// TODO: Make this work. * See above 
		//m_model.save("trained_models/trained_nn.xml");
	}

	////Eigen::Tensor<float, 2> NeuralNetworkHeuristic::featureExtraction(const Position & pos) 
	////{
	////	// --- Alias ---
	////	Board b = (pos.moveCounter().isWhitesTurn() ? pos.board() : pos.board().rotated());
	////
	////	BitBoard ours = b.whites();
	////	BitBoard theirs = b.blacks();
	////
	////	BitBoard ourKings = ours & b.kings();
	////	BitBoard ourQueens = ours & b.queens();
	////	BitBoard ourBishops = ours & b.bishops();
	////	BitBoard ourKnights = ours & b.knights();
	////	BitBoard ourRooks = ours & b.rooks();
	////	BitBoard ourPawns = ours & b.pawns();
	////
	////	BitBoard theirKings = theirs & b.kings();
	////	BitBoard theirQueens = theirs & b.queens();
	////	BitBoard theirBishops = theirs & b.bishops();
	////	BitBoard theirKnights = theirs & b.knights();
	////	BitBoard theirRooks = theirs & b.rooks();
	////	BitBoard theirPawns = theirs & b.pawns();
	////
	////	Eigen::Tensor<float, 2> inputs(1, this->nInputNodes());
	////	
	////	for (size_t bit = 0; bit < 64; bit++) {
	////		// TODO: Optimize: Can this be optimized with ifs. Hint: Sparse data.
	////		inputs(1, 64 * 0 + bit) = ourKings[bit];
	////		inputs(1, 64 * 1 + bit) = ourQueens[bit];
	////		inputs(1, 64 * 2 + bit) = ourBishops[bit];
	////		inputs(1, 64 * 3 + bit) = ourKnights[bit];
	////		inputs(1, 64 * 4 + bit) = ourRooks[bit];
	////		inputs(1, 64 * 5 + bit) = ourPawns[bit];
	////		inputs(1, 64 * 6 + bit) = theirKings[bit];
	////		inputs(1, 64 * 7 + bit) = theirQueens[bit];
	////		inputs(1, 64 * 8 + bit) = theirBishops[bit];
	////		inputs(1, 64 * 9 + bit) = theirKnights[bit];
	////		inputs(1, 64 * 10 + bit) = theirRooks[bit];
	////		inputs(1, 64 * 11 + bit) = theirPawns[bit];
	////	}
	////
	////	return inputs;
	////}

} // namespace forge