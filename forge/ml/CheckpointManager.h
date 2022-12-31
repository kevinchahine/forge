#pragma once

#include <iostream>
#include <string>
#include <filesystem>

namespace forge
{
	namespace ml
	{
		class CheckpointManager
		{
		public:
			void checkpointDir(const std::filesystem::path& dir);
			const std::filesystem::path& checkpointDir() const { return _checkpointDir; }

			std::filesystem::path networkDir() const;
				
			void name(const std::string& name) { _name = name; }
			const std::string& name() const { return _name; }

			size_t countCheckpoints() const;

			// Searches directory for the latest checkpoint.
			// Latest checkpoint is determined by the postfix index.
			// Returns its absolute path.
			std::filesystem::path latest() const;

			// Determines a filename where the next checkpoint can be saved.
			// This method will generate a file name which does not yet exist.
			// It can be created when saving a checkpoint.
			std::filesystem::path generateFilename(int epoch = -1) const;

		private:
			std::filesystem::path _checkpointDir;
			std::filesystem::path _ext = ".pt";// extension of checkpoint file (ex: .pt) TODO: 
			std::string _name = "default";
		};
	} // namespace ml
} // namespace forge