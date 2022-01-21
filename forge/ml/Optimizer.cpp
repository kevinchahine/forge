#include "Optimizer.h"

#include "../Position.h"

#include "../rapidcsv.h"

#include <boost/filesystem.hpp>

#include <fstream>
#include <sstream>
#include <assert.h>
#include <chrono>

#include <opencv4/opencv2/opencv.hpp>

using namespace std;
namespace csv = rapidcsv;

namespace forge
{
	namespace ml 
	{
		// ---------------------------- FUNCTIONS -----------------------------
		
		template <typename PIECE_T, size_t offset>
		void bbToOneHot(cv::Mat & samples, size_t row, const Board & board) 
		{
			BitBoard bb = board.pieces<PIECE_T>();
			for (size_t bit = 0; bit < bb.size(); bit++) {
				samples.at<float>(row, offset + bit) = bb[bit];
			}
		}

		// ---------------------------- METHODS -------------------------------

		cv::Ptr<cv::ml::ANN_MLP> Optimizer::createNN()
		{
			int nFeatures = 64 * 12;

			cv::Mat_<int> layerSizes(4, 1);	// 4 layers, 1 is like a placeholder
			layerSizes(0) = nFeatures;		// input
			layerSizes(1) = 512;			// hidden1
			layerSizes(2) = 32;				// hidden2
			layerSizes(3) = 1;				// output

			cv::Ptr<cv::ml::ANN_MLP> ann = cv::ml::ANN_MLP::create();
			ann->setLayerSizes(layerSizes);
			ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 0, 0);
			ann->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 500, 0.0001));	// train for 10'000 iterations (samples)
			ann->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.0001);
			//ann->setPreferableBackend(cv::DNN_BACKEND_CUDA);
			//ann->setPreferableTarget(cv::DNN_TARGET_CUDA);

			// TODO: See if setting MAX_ITER TO infinity changes anything.

			return ann;
		}
		
		deque<PosEvalPair> Optimizer::loadDatasetCSV(const boost::filesystem::path & datasetFile)
		{
			// 1.) --- Precheck ---
			if (boost::filesystem::exists(datasetFile)) {
				if (boost::filesystem::is_regular_file(datasetFile)) {
					if (datasetFile.extension() == ".csv") {
					}
					else {
						stringstream ss;
						ss << "File '" << datasetFile << "' must be a .csv file.";
						throw runtime_error(ss.str());
					}
				}
				else {
					stringstream ss;
					ss << "File '" << datasetFile << "' must be a regular file.";
					throw runtime_error(ss.str());
				}
			}
			else {
				stringstream ss;
				ss << "File '" << datasetFile << "' was not found. Make sure it exists.";
				throw runtime_error(ss.str());
			}

			
			// 2.) --- Read File ---
			ifstream in(datasetFile.string());

			deque<PosEvalPair> ds = loadDatasetCSV(in);

			return ds;
		}

		deque<PosEvalPair> Optimizer::loadDatasetCSV(istream & in, size_t nlines) 
		{
			deque<PosEvalPair> ds;

			// Read `nlines` lines or till the end of the file
			for (size_t l = 0; l < nlines; l++) {
				// --- Read entire Line ---
				string line;
				getline(in, line);

				// Did we reach the end of the file?
				if (line.empty()) {
					break;
				}

				// Skip the header
				if (line.size() < 20) {
					continue;
				}

				// --- Tokenize ---
				istringstream ss(move(line));
				string fen;
				int eval;

				getline(ss, fen, ',');		// extract FEN
				ss >> eval;					// extract EVAL

				// --- Push into dataset ---
				ds.emplace_back(
					Position(),		// Empty Position
					eval);			// Eval as an int

				ds.back().pos.fromFEN(fen);	// Set Position from FEN
			}
			
			return ds;
		}

		cv::Ptr<cv::ml::TrainData> Optimizer::preprocess(const deque<PosEvalPair> & posEvalPairs)
		{
			assert(posEvalPairs.size() > 0);

			size_t nSamples = posEvalPairs.size();	// Number of training samples we have
			size_t nFeatures = 12;					// Number of features to extract from samples 
													// Each feature will become a One-Hot encoding of 64-elements
			size_t nColumns = nFeatures * 64;		// 

			// TODO: Type might have to be CV_32F
			// Each row is a training sample.
			// Each column represents a feature of the sample
			// Columns (each 64 columns in a one-hot encoding):
			//	1st	 64: WHITE KING   
			//	2nd	 64: WHITE QUEEN  
			//	3rd	 64: WHITE BISHOP 
			//	4th	 64: WHITE KNIGHT 
			//	5th	 64: WHITE ROOK   
			//	6th	 64: WHITE PAWN   
			//	7th	 64: BLACK KING   
			//	8th	 64: BLACK QUEEN  
			//	9th	 64: BLACK BISHOP 
			//	10th 64: BLACK KNIGHT 
			//	11th 64: BLACK ROOK   
			//	12th 64: BLACK PAWN   
			cv::Mat samples = cv::Mat::zeros(nSamples, nColumns, CV_32F);
			cv::Mat responses = cv::Mat::zeros(nSamples, 1, CV_32F);

			//cout << "Samples shape: " << samples.rows << " x " << samples.cols << endl;

			auto start = chrono::high_resolution_clock::now();
			// --- Initialize One-Hot Encodings ---
			for (size_t row = 0; row < posEvalPairs.size(); row++) {
				const PosEvalPair & pair = posEvalPairs.at(row);
				const Position & pos = pair.pos;
				const Board & board = pos.board();

				// --- Samples ---
				bbToOneHot<pieces::WhiteKing,	0>(samples, row, board);
				bbToOneHot<pieces::WhiteQueen,	64>(samples, row, board);
				bbToOneHot<pieces::WhiteBishop,	128>(samples, row, board);
				bbToOneHot<pieces::WhiteKnight,	192>(samples, row, board);
				bbToOneHot<pieces::WhiteRook,	256>(samples, row, board);
				bbToOneHot<pieces::WhitePawn,	320>(samples, row, board);
				bbToOneHot<pieces::BlackKing,	384>(samples, row, board);
				bbToOneHot<pieces::BlackQueen,	448>(samples, row, board);
				bbToOneHot<pieces::BlackBishop,	512>(samples, row, board);
				bbToOneHot<pieces::BlackKnight,	576>(samples, row, board);
				bbToOneHot<pieces::BlackRook,	640>(samples, row, board);
				bbToOneHot<pieces::BlackPawn,	704>(samples, row, board);

				// --- Responses ---
				responses.at<float>(row, 0) = pair.eval;		
			}

			auto stop = chrono::high_resolution_clock::now();
			cout << "done. " << chrono::duration<double, ratio<1,1>>(stop - start).count() << " sec" << endl;

			//cv::imshow("1243", samples);
			//cv::waitKey(0);

			return cv::ml::TrainData::create(
				samples,
				cv::ml::SampleTypes::ROW_SAMPLE,	// Reach row is a sample
				responses
			);
		}

		float Optimizer::validate(cv::Ptr<cv::ml::ANN_MLP> & ann, const cv::Ptr<cv::ml::TrainData> & trainData)
		{
			auto testSamples = trainData->getTestSamples();
			auto testResponses = trainData->getTestResponses();

			cv::Mat predictions;
			ann->predict(testSamples, predictions);

			cout << "TestResponses shape: " << testResponses.rows << " x " << testResponses.cols << endl;
			cout << "Predictions   shape: " << predictions.rows << " x " << predictions.cols << endl;
			// Calculate MSE of predictions
			float error_sum = 0.0f;
			for (int i = 0; i < predictions.rows; i++) {
				float error = predictions.at<float>(i, 0) - testSamples.at<float>(i, 0);

				error_sum += error * error;
			}

			cout << "Avg MSE: " << error_sum / predictions.rows << " over " << predictions.rows << " samples" << endl;	
		}

		void Optimizer::save(cv::Ptr<cv::ml::ANN_MLP> & ann, const boost::filesystem::path & annFile)
		{
			// https://answers.opencv.org/question/175766/how-to-save-ann_mlp-by-appending-to-file-containing-other-data/
			cv::FileStorage fs;
			fs.open(annFile.string(), cv::FileStorage::WRITE);
			ann->write(fs);
			fs.release();
		}

		cv::Ptr<cv::ml::ANN_MLP> Optimizer::load(const boost::filesystem::path & annFile)
		{
			cv::Ptr<cv::ml::ANN_MLP> ann = cv::ml::ANN_MLP::create();
			
			cv::FileStorage fs;
			fs.open(annFile.string(), cv::FileStorage::READ);
			ann->read(fs.root());
			fs.release();

			return ann;
		}

		void Optimizer::train()
		{
			boost::filesystem::path dsPath("/media/kevin/barracuda/Datasets/Chess/chessData-mod.csv");
			ifstream infile(dsPath.string());

			cv::Ptr<cv::ml::ANN_MLP> ann = 
				//Optimizer::createNN();
				Optimizer::load("model_4");

			int trainCounter = 0;
			while (infile.good()) {
				cout << "--- Training Counter: " << trainCounter << " ---" << endl;
				
				// 1.) Load Dataset
				cout << "--- Loading Dataset ---" << endl;
				deque<PosEvalPair> ds = Optimizer::loadDatasetCSV(infile, 500); //10'000);	// make sure 10'000 matches TermCriteria(). See createNN().

				// 2.) Break when we reach the end of the file
				if (ds.empty()) {
					break;
				}

				// 3.) Preprocessing
				cout << "--- Preprocessing ---" << endl;
				cv::Ptr<cv::ml::TrainData> trainData = Optimizer::preprocess(ds);
				trainData->setTrainTestSplitRatio(0.95, true);

				// 4.) Train Model
				cout << "--- Training ---" << endl;
				ann->train(trainData);

				// 5.) Validate Model
				cout << "--- Validating ---" << endl;
				float mse = Optimizer::validate(ann, trainData);
				cout << "MSE: " << mse << endl;

				// 6.) Save Model
				cout << "--- Saving to File ---" << endl;
				boost::filesystem::path modelFile = "model_" + to_string(trainCounter);
				Optimizer::save(ann, modelFile);

				trainCounter++;
			}
		}

	} // namespace ml
} // namespace forge