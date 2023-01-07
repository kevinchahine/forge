#pragma once

#include <iostream>
#include <filesystem>

namespace forge
{
	namespace drivers
	{
		// Splits a csv dataset into 2 files (train, test).
		// Creates 2 csv files.
		// Samples are shuffled then split.
		// percent - proportion of dataset to be used for training 
		//	ex: percent = 90.0f will put 90% of samples in training and 10% in testing
		void split_dataset(float percent, const std::filesystem::path & original_ds);

	} // namespace drivers
} // namespace forge