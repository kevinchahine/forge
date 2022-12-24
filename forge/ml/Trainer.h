#pragma once

#include <filesystem>

#include "forge/ml/StockfishDataset.h"

namespace forge
{
	namespace ml
	{
		class Trainer
		{
		public:
			void checkpointDir(const std::filesystem::path& dir) { _checkpointDir = dir; }
			const std::filesystem::path& checkpointDir() const { return _checkpointDir; }

			void train();

		private:
			std::filesystem::path _checkpointDir;
		};
	} // namespace ml
} // namespace forge