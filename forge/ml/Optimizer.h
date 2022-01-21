#pragma once

#include "PosEvalPair.h"

#include <iostream>
#include <string>
#include <deque>

#include <boost/filesystem/path.hpp>

#include <opencv4/opencv2/ml.hpp>

namespace forge
{
	namespace ml 
	{
		/*
			Use cases:
				Optimizer op;

				auto fenDS = op.loadDatasetCSV("data.csv");

				op.train();

				op.getNN();
		*/
		class Optimizer
		{
		protected:
			static cv::Ptr<cv::ml::ANN_MLP> createNN();
			
			// Loads a csv file into a container of PosEvalPair
			// csv file must have 2 columns (both stored in plain text):
			//	1st column: FEN as a string
			//	2nd column: EVAL as an int
			static std::deque<PosEvalPair> loadDatasetCSV(const boost::filesystem::path & datasetFile);

			static std::deque<PosEvalPair> loadDatasetCSV(std::istream & in, size_t nlines = numeric_limits<size_t>::max());

			static cv::Ptr<cv::ml::TrainData> preprocess(const std::deque<PosEvalPair> & posEvalPairs);

			static float validate(cv::Ptr<cv::ml::ANN_MLP> & ann, const cv::Ptr<cv::ml::TrainData> & trainData);

			static void save(cv::Ptr<cv::ml::ANN_MLP> & ann, const boost::filesystem::path & annFile);

			static cv::Ptr<cv::ml::ANN_MLP> load(const boost::filesystem::path & annFile);

		public:
			void train();

		private:

		}; // class Optimizer
	} // namespace ml
} // namespace forge