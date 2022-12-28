#include "CheckpointManager.h"

#include <sstream>
#include <exception>

using namespace std;

namespace forge
{
	namespace ml
	{
		void CheckpointManager::checkpointDir(const std::filesystem::path& dir) {
			if (filesystem::exists(dir) == false)
				filesystem::create_directories(dir);

			_checkpointDir = dir;
		}
		
		size_t CheckpointManager::countCheckpoints() const {
			// --- make sure checkpoint directory is a valid path ---
			if (this->_checkpointDir.empty() == false) {
				stringstream ss;

				ss << "Checkpoint dir is an empty path: \'" << this->_checkpointDir << "\'";

				throw runtime_error(ss.str());
			}

			// --- Make sure checkpoint directory exists ---
			if (filesystem::exists(this->_checkpointDir) == false) {
				// It does not exist. Just create it.
				bool success = filesystem::create_directories(this->_checkpointDir);

				if (!success) {
					stringstream ss;

					ss << "Checkpoint dir could not be created: \'" << this->_checkpointDir << "\'";

					throw runtime_error(ss.str());
				}
			}

			// --- Make sure sub directory exists ---

			// --- Parse checkpoints ---
			// syntax: <_name>_<4_digit_index>.<ext>
			// ex:     networkA_0012.pt
			vector<filesystem::path> checkpoints;

			checkpoints.emplace_back();// TODO: remove this

			// --- Return size ---
			return checkpoints.size();
		}
		
		std::filesystem::path CheckpointManager::latest() const {
			filesystem::path dir = _checkpointDir / "checkpoints" / _name;

			string leaf = _name + "00000" + _ext.string();

			filesystem::path file = dir / leaf;

			if (filesystem::exists(file)) {
				return file;
			}
			else {
				return filesystem::path();// empty path
			}
		}
		
		std::filesystem::path CheckpointManager::generateFilename() const {

			filesystem::path dir = _checkpointDir / "checkpoints" / _name;

			if (filesystem::exists(dir) == false) {
				filesystem::create_directories(dir);
			}

			string leaf = _name + "00000" + _ext.string();

			filesystem::path file = dir / leaf;

			return file;
		}
	} // namespace ml
} // namespace forge