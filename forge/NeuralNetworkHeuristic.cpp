#include "NeuralNetworkHeuristic.h"

using namespace std;

namespace forge
{
	NeuralNetworkHeuristic::NeuralNetworkHeuristic() :
		m_model(OpenNN::NeuralNetwork(OpenNN::NeuralNetwork::ProjectType::Approximation, {10, 3, 2, 1}))
	{}
	
	//NeuralNetworkHeuristic::NeuralNetworkHeuristic(const std::string& model_file_name)
	//{
	//	cout << __FUNCTION__ << " not implemented\n";
	//	//m_model = ml::Optimizer::createNN();
	//}

	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		Eigen::Tensor<float, 2> inputs(1, 10);
		Eigen::Tensor<float, 2> outputs(1, 1);
		outputs(0, 0) = 5;

		for (int i = 0; i < 10; i++) {
			inputs(0, i) = i;
		}
		
		// --- Preprocess ---

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

	// Assigns the hot-encoding of a BitBoard to a specified row in a cv::Mat
	// 
	// Parameters:
	//		PIECE_T:
	//			BitBoard comes from `board` and is specified by `PIECE_T`
	//			`PIECE_T` should be one of the piece types found under namespace pieces
	//				ex: peices::Pawn, pieces::Knight, (not pieces::WhitePawn)
	//				Do Not use types that specify color or direction. Only use
	//				King, Queen, Bishop, Knight, Rook, Pawn
	//		mat:
	//			a cv::Mat of size 12 rows, 64 columns
	//			Each row stores the one hot encoding of a piece
	//		board:
	//			board of the current Position
	//		ours:
	//			BitBoard of all 'our' pieces. Use board.whites() or board.blacks()
	//		theirs:
	//			BitBoard of all 'their' pieces. Use board.whites() or board.blacks()
	//		rowIndex:
	//			Each row in `mat` stores the one-hot encoding for a specific piece.
	//			`rowIndex` specifies the index that the one-hot encoding is to be stored into.
	//			For convenience and because each piece can be ours or theirs, this function will 
	//			store the one-hot encoding of our peices at `rowIndex` and their pieces at `rowIndex + 1`
	//		Also, when ours is black pieces, the board will be flipped to be in perspective of black.

	// Does what BitBoardToCvMat does but flips indices of each piece so that 
	// coordinates are in the perspective of the black player.
	
//	cv::Mat NeuralNetworkHeuristic::preprocess(const Position& pos) const
//	{
//		const Board& b = pos.board();
//
//		// ----- Who's Turn? -----
//
//		BitBoard ours, theirs;
//		BoardSquare ourKing, theirKing;
//		size_t ourKingIndex, theirKingIndex;
//
//		// --- Determine Who's Turn It Is ---
//		if (pos.moveCounter().isWhitesTurn()) {
//			// It is WHITES turn
//			ours = b.whites();
//			theirs = b.blacks();
//			ourKing = b.whiteKing();
//			theirKing = b.blackKing();
//			ourKingIndex = ourKing.index();
//			theirKingIndex = theirKing.index();
//
//			// ----- Create Hot-Encodings of each feature -----
//		}
//		else {
//			// It is BLACKS turn
//			ours = b.blacks();
//			theirs = b.whites();
//			ourKing = b.blackKing();
//			theirKing = b.whiteKing();
//			ourKingIndex = 63 - ourKing.index();
//			theirKingIndex = 63 - theirKing.index();
//		
//			// ----- Create Hot-Encodings of each feature -----
//		}
//	}
} // namespace forge