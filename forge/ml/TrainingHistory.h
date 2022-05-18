#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>

namespace forge
{
	namespace ml
	{
		class LossPoint 
		{
		public:
			// size_t	float	float
			// <epoch>	<train>	<valid>
			// 23 2.3 4.5
			void from_string(const std::string & str)
			{
				std::istringstream iss(str);

				iss >> epoch >> train >> valid;
			}

		public:
			size_t epoch;	// epoch number
			float train;	// training loss
			float valid;	// validation loss
		};

		/*
			TrainingHistory h;
			h.fromFile("history.txt");
			h.setOutFile("history.txt");

			h.emplace_back(0, 3.4, 5.6);
			h.emplace_back(1, 3.4, 5.6);
			h.emplace_back(2, 3.4, 5.6);
			h.emplace_back(3, 3.4, 5.6);
			h.emplace_back(4, 3.4, 5.6);

			h.flush();

			h.close();

			// ----------------

			TrainingHistory h2;

			h2.
		*/

		class TrainingHistory : public std::deque<LossPoint>
		{
		public:
			// Loads data from a previously saved history file
			// and inserts into deque. If called multiple times, 
			// then data will continue being appended.
			void from(const std::string & filename);

			// See above *
			void from(std::ifstream & inFile);

			// Sets output file to set 
			void setOutFile(const std::string & filename);

			// move op
			void setOutFile(std::ofstream && outFile);

			void push_back(const LossPoint & loss);

			void emplace_back(LossPoint && loss);

			void flush();

			void plot();

			void getPlot();

		private:
			std::ofstream outFile;
		};
	} // namespace ml
} // namespace forge