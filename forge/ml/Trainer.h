#pragma once

#include <filesystem>

#include "forge/ml/StockfishDataset.h"
#include "forge/ml/Network.h"
#include "forge/ml/CheckpointManager.h"

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

			void test();

		private:
			void init();

		private:
			std::filesystem::path _checkpointDir;

			CheckpointManager _checkpoint;

			StockfishDataset _dataset;

			NetworkE _network;
		};
	} // namespace ml
} // namespace forge