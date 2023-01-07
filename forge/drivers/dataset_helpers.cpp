#include "dataset_helpers.h"
#include "forge/source/globals.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace forge
{
	namespace drivers
	{
		// Generates a filename based on 'original_ds'
		// Appends 'postfix' to the filename leaf before the extension
		filesystem::path generate_file_name(const filesystem::path & original_ds, const string & postfix) {
			string stem = original_ds.stem().string() + "_" + postfix;

			filesystem::path filename = original_ds.parent_path() / stem;
			filename.replace_extension(original_ds.extension());

			return filename;
		}

		void split_dataset(float percent, const filesystem::path & original_ds) {
			// --- 1.) Make sure dataset exists in filesystem ---
			if (filesystem::exists(original_ds) == false) {
				stringstream ss;

				ss << "Error: " << __FILE__ << " line " << __LINE__ 
					<< ": Dataset file: " << original_ds.generic_string() 
					<< " does not exist";

				throw std::runtime_error(ss.str());
			}

			// --- 2.) Open datasets ---
			ifstream in;
			in.open(original_ds);
			string line;
			getline(in, line);// ignore csv header

			filesystem::path trainFile = generate_file_name(original_ds, "train");
			ofstream trainOut;
			trainOut.open(trainFile);
			cout  << "Saving to " << trainFile.generic_string() << endl;

			filesystem::path testFile = generate_file_name(original_ds, "test");
			ofstream testOut;
			testOut.open(testFile);
			cout << "And " << testFile.generic_string() << endl;

			// --- 3.) Load Batches ---

			vector<string> samples;
			size_t batchSize = 1'000;

			do {
				// --- 3-0.) Load Batch ---
				samples.clear();
				samples.reserve(batchSize);

				for (size_t s = 0; s < batchSize && getline(in, line); s++) {
					samples.emplace_back(std::move(line));
				}
				
				// --- 3-1.) Shuffle Batch ---
				shuffle(samples.begin(), samples.end(), g_rand);

				// --- 3-2.) Split Batch ---
				size_t splitPoint = (percent * samples.size()) / 100;
				
				// --- 3-3.) Write to Train File ---
				for (size_t i = 0; i < splitPoint; i++) {
					trainOut << samples.at(i) << endl;
				}

				// --- 3-4.) Write to Test File ---
				for (size_t i = splitPoint; i < samples.size(); i++) {
					testOut << samples.at(i) << endl;
				}
			} while (samples.size());
		}
	}
}