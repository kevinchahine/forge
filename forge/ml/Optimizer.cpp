#include "Optimizer.h"

#include "../Position.h"

#include "../rapidcsv.h"

#include <boost/filesystem.hpp>

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

			cout << "Reading...";	cout.flush();

			auto start = chrono::high_resolution_clock::now();
			rapidcsv::Document doc(datasetFile.string());
			auto stop = chrono::high_resolution_clock::now();

			cout << "done. " << chrono::duration<double, ratio<1,1>>(stop - start).count() << " sec" << endl;

			cout << "rows: " << doc.GetRowCount() << endl
			<< "cols: " << doc.GetColumnCount() << endl;

			// 3.) --- Parse data ---
			deque<PosEvalPair> ds;
			
			cout << "Converting FEN To Positions...";

			start = chrono::high_resolution_clock::now();
			size_t nLines = doc.GetRowCount();
			for (size_t row = 0; row < nLines; row++) {
				// TODO: Put some error checking here to make sure types match
				
				ds.emplace_back(
					Position(),					// Empty Position
					doc.GetCell<int>(1, row));	// Eval as an int

				ds.back().pos.fromFEN(doc.GetCell<string>(0, row));	// Set Position from FEN
			}
			stop = chrono::high_resolution_clock::now();

			cout << "done. " << chrono::duration<double, ratio<1,1>>(stop - start).count() << " sec" << endl;

			// 4.) --- Return as a queue of PosEvalPairs ---
			return ds;
		}

		cv::Ptr<cv::ml::TrainData> Optimizer::preprocess(const deque<PosEvalPair> & posEvalPairs)
		{
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
			// Column Matrix of responses (targets)
			cv::Mat responses = cv::Mat::zeros(nSamples, 1, CV_32F);
			cout << samples.rows << " x " << samples.cols << endl;
			assert(samples.rows == responses.rows);	 // "Samples and Responses must have the same number of rows"

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

		void Optimizer::train()
		{
			boost::filesystem::path dsPath("/media/kevin/barracuda/Datasets/Chess/chessData-mod-head.csv");

			// 1.) Load Dataset
			deque<PosEvalPair> ds = Optimizer::loadDatasetCSV(dsPath);

			// 2.) Preprocessing
			cv::Ptr<cv::ml::TrainData> trainData = Optimizer::preprocess(ds);

			
		}

	} // namespace ml
} // namespace forge