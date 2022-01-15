#include "NeuralNetworkHeuristic.h"

using namespace std;

namespace forge
{
	NeuralNetworkHeuristic::NeuralNetworkHeuristic(const std::string& model_file_name)
	{

	}

	heuristic_t NeuralNetworkHeuristic::eval(const Position& pos)
	{
		// --- Preprocess ---
		cv::Mat input = this->preprocess(pos);
		//cv::InputArrayOfArrays input;	// Consider using this instead

		// --- Feed Forward ---
		vector<int> output{ 1, 1 };
		//m_model.predict(input, output);

		// --- Return ---
		heuristic_t evaluation = 0;
		
		evaluation = output.front();

		return evaluation;
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
	template<typename PIECE_T>
	void BitBoardToCvMat(cv::Mat& mat, const Board& board, const BitBoard & ours, const BitBoard & theirs, int rowIndex) 
	{
		////////////BitBoard pieces = board.pieces<PIECE_T>();
		////////////BitBoard ourPieces = pieces & ours;
		////////////BitBoard theirPieces = pieces & theirs;
////////////
		////////////for (size_t index = 0; index < 64; index++) {
		////////////	if (ourPieces[index] == 1) {
		////////////		mat.at<int8_t>(cv::Point(index, rowIndex)) = 1;
		////////////	}
		////////////	else if (theirPieces[index] == 1) {
		////////////		mat.at<int8_t>(cv::Point(index, rowIndex + 1)) = 1;
		////////////	}
		////////////}
	}

	// Does what BitBoardToCvMat does but flips indices of each piece so that 
	// coordinates are in the perspective of the black player.
	template<typename PIECE_T>
	void BitBoardToCvMatFlipped(cv::Mat& mat, const Board& board, const BitBoard& ours, const BitBoard& theirs, int rowIndex)
	{
		/////////BitBoard pieces = board.pieces<PIECE_T>();
		/////////BitBoard ourPieces = pieces & ours;
		/////////BitBoard theirPieces = pieces & theirs;
/////////
		/////////for (size_t index = 0; index < 64; index++) {
		/////////	auto flippedIndex = 64 - index;
/////////
		/////////	if (ourPieces[index] == 1) {
		/////////		mat.at<int8_t>(cv::Point(flippedIndex, rowIndex)) = 1;
		/////////	}
		/////////	else if (theirPieces[index] == 1) {
		/////////		mat.at<int8_t>(cv::Point(flippedIndex, rowIndex + 1)) = 1;
		/////////	}
		/////////}
	}
	
	cv::Mat NeuralNetworkHeuristic::preprocess(const Position& pos) const
	{
		cv::Mat mat = cv::Mat::zeros(12, 64, CV_8S);
		
		const Board& b = pos.board();

		// ----- Who's Turn? -----

		BitBoard ours, theirs;
		BoardSquare ourKing, theirKing;
		size_t ourKingIndex, theirKingIndex;

		// --- Determine Who's Turn It Is ---
		if (pos.moveCounter().isWhitesTurn()) {
			// It is WHITES turn
			ours = b.whites();
			theirs = b.blacks();
			ourKing = b.whiteKing();
			theirKing = b.blackKing();
			ourKingIndex = ourKing.index();
			theirKingIndex = theirKing.index();

			// ----- Create Hot-Encodings of each feature -----
			BitBoardToCvMat<pieces::Pawn>(mat, b, ours, theirs, 0);
			BitBoardToCvMat<pieces::Knight>(mat, b, ours, theirs, 2);
			BitBoardToCvMat<pieces::Bishop>(mat, b, ours, theirs, 4);
			BitBoardToCvMat<pieces::Rook>(mat, b, ours, theirs, 6);
			BitBoardToCvMat<pieces::Queen>(mat, b, ours, theirs, 8);
			mat.at<int8_t>(ourKingIndex, 10) = 1;
			mat.at<int8_t>(theirKingIndex, 11) = 1;
		}
		else {
			// It is BLACKS turn
			ours = b.blacks();
			theirs = b.whites();
			ourKing = b.blackKing();
			theirKing = b.whiteKing();
			ourKingIndex = 63 - ourKing.index();
			theirKingIndex = 63 - theirKing.index();
		
			// ----- Create Hot-Encodings of each feature -----
			BitBoardToCvMatFlipped<pieces::Pawn>(mat, b, ours, theirs, 0);
			BitBoardToCvMatFlipped<pieces::Knight>(mat, b, ours, theirs, 2);
			BitBoardToCvMatFlipped<pieces::Bishop>(mat, b, ours, theirs, 4);
			BitBoardToCvMatFlipped<pieces::Rook>(mat, b, ours, theirs, 6);
			BitBoardToCvMatFlipped<pieces::Queen>(mat, b, ours, theirs, 8);
			mat.at<int8_t>(ourKingIndex, 10) = 1;
			mat.at<int8_t>(theirKingIndex, 11) = 1;
		}

		return mat;
	}
} // namespace forge