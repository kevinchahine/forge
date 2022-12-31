#include "CheckpointManager.h"

#include <sstream>
#include <iomanip>
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

		std::filesystem::path CheckpointManager::networkDir() const {
			return _checkpointDir / "checkpoints" / _name;
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
			filesystem::path dir = networkDir();

			stringstream leaf;

			leaf << _name << "_";

			//leaf << "default";
			leaf << right << "00000";

			leaf << _ext.string();

			filesystem::path file = dir / leaf.str();

			if (filesystem::exists(file)) {
				return file;
			}
			else {
				return filesystem::path();// empty path
			}
		}
		
		std::filesystem::path CheckpointManager::generateFilename(int epoch) const {

			filesystem::path dir = networkDir();

			if (filesystem::exists(dir) == false) {
				filesystem::create_directories(dir);
			}

			stringstream leaf;

			leaf << _name << "_";

			if (epoch == -1) {
				leaf << "default";
			}
			else {
				leaf << right << setfill('0') << setw(5) << epoch;
			}
				
			leaf << _ext.string();

			filesystem::path file = dir / leaf.str();

			return file;
		}
	} // namespace ml
} // namespace forge