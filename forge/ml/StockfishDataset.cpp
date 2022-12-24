#include <fstream>
#include <sstream>
#include <chrono>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

#include "forge/ml/StockfishDataset.h"
#include "forge/heuristic/FeatureExtractor.h"
#include "forge/source/globals_torch.h"

using namespace std;

namespace forge
{
	namespace ml
	{
		StockfishDataset::FenEval StockfishDataset::FenEval::parse(const std::string& line) {
			// Each line is in this format:
			//		<fen>,<eval>
			// were:
			//	<fen> - the position in fen 
			//	<eval> - integer evaluation

			FenEval obj;

			boost::regex regex =
				boost::regex(R"dil(\s*([KkQqRrBbNnPp\d\/]+\s+[BbWw]\s+[KQkq-]+\s+[-abcdefgh\d]+\s+\d+\s+\d+)\s*,\s*#{0,1}\s*([-+0123456789]+))dil");
			
			boost::sregex_token_iterator regex_it =
				boost::sregex_token_iterator(line.begin(), line.end(), regex, { 1, 2 });

			boost::sregex_token_iterator end;

			if (regex_it != end) {
				obj.fen = *regex_it++;
			}
			else {
				obj.fen.clear();
				return obj;
			}

			if (regex_it != end) {
				stringstream ss(*regex_it++);
				ss >> obj.eval;
			}
			else {
				obj.fen.clear();
				return obj;
			}

			return obj;
		}

		size_t fileRead(istream& is, vector<char>& buff) {
			is.read(&buff[0], buff.size());
			return is.gcount();
		}

		size_t countLines(const vector<char>& buff, int sz) {
			int newlines = 0;
			const char* p = &buff[0];
			for (int i = 0; i < sz; i++) {
				if (p[i] == '\n') {
					newlines++;
				}
			}
			return newlines;
		}

		// --------------------------------------------------------------------

		StockfishDataset::StockfishDataset(const StockfishDataset& ds) :
			_batch(ds._batch),
			_csvFile(ds._csvFile),
			_batchSize(ds._batchSize),
			_lineCount(ds._lineCount),
			_nSamples(ds._nSamples) {

			this->open(ds._csvFile);
			
			skip(_lineCount);
		}

		StockfishDataset& StockfishDataset::operator=(const StockfishDataset& ds) {
			_batch = ds._batch;
			_csvFile = ds._csvFile;
			_batchSize = ds._batchSize;
			_lineCount = ds._lineCount;
			_nSamples = ds._nSamples;

			this->open(ds._csvFile);
			
			skip(_lineCount);

			return (*this);
		}

		void StockfishDataset::open(const std::filesystem::path& csvPath) {
			// --- CSV File? ---
			_csvFile = csvPath;

			if (_csvFile.extension() != ".csv") {
				cout << "Error: " << _csvFile << " is not a .csv file" << endl;
				return;
			}

			// --- In File Stream ---

			size_t count = 0;

			_in.open(_csvFile);
			if (_in.is_open()) {
				//cout << "File " << _csvFile << " openned sucessfully :)" << endl;
			}
			else {
				cout << "Error: Could not open " << _csvFile << ". Please make sure the file exists. :(" << endl;
				return;
			}

			// --- Count how many lines of data we have ---
			countSamples();
		}

		void StockfishDataset::skip(size_t nLines) {
			string line;

			size_t count = 0;

			while (getline(_in, line) && count < nLines) {
				// !!! Always skip the 1st line (csv header) !!!
				count++;
			}
		}

		torch::data::Example<> StockfishDataset::get(size_t index) {
			// --- Do we need to load another batch? ---
			if (_batchIt >= _batch.nSamples()) {
				//cout << "Loading another batch" << endl;
				load();
			}

			if (_batch.nSamples()  == 0) {
				cout << "Error: dataset is empty even after load()" << endl;
				return torch::data::Example<>{};
			}

			// --- Next Element ---
			_lineCount++;
			
			// --- Position --- 
			torch::Tensor input = _batch.input[_batchIt];

			// --- Evaluation ---
			torch::Tensor output = _batch.output[_batchIt];

			_batchIt++;

			return { input, output };
		}

		void StockfishDataset::countSamples() {

			chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

			const int SZ = 1024 * 1024;

			vector<char> buff(SZ);

			ifstream inFile;
			inFile.open(_csvFile);

			size_t count = 0;

			while (int cc = fileRead(inFile, buff)) {
				count += countLines(buff, cc);
			}

			chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();

			cout << "Counting lines took " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() 
				<< " milliseconds" << endl;

			_nSamples = count;
		}

		void StockfishDataset::load() {
			// --- Reset Tensors ---
			_batch.input = torch::zeros({ _batchSize, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE }, torch::kCPU);
			_batch.output = torch::zeros({ _batchSize, 1 }, torch::kCPU);
			_batchIt = 0;

			// --- Parse Lines ---
			string line;
			size_t count = 0;

			while (getline(_in, line) && count < _batchSize) {
				try {
					// --- Parse Line info FEN and int ---
					FenEval fe = FenEval::parse(line);

					if (fe.isInValid()) {
						stringstream ss;
						ss << "Line could not be parsed: " << line;
						throw std::runtime_error(ss.str());
					}
					
					// --- Convert FEN to Position ---
					Position pos;
					pos.fromFEN(fe.fen);

					// --- Convert To Tensors ---

					// -- Input Tensor --
					forge::heuristic::FeatureExtractor extractor;
					extractor.init(pos, forge::WHITE);// Extract features from whites perspective
					// *** Whites on the bottom moving up ***
					torch::Tensor sampleSlice = _batch.input.slice(0, count, count + 1);
					extractor.extractMaterial(sampleSlice);
					// -- Output Tensor --
					_batch.output[count] = fe.eval;

				} catch (const std::exception& e) {
					cout << "Exception _in " << __FILE__ << " line " << __LINE__ << ": " << e.what() << endl
						<< '\t' << line << endl;
					cin.get();
				}

				count++;
			}

			// --- Move Batch to Training Device ---
			_batch.to(forge::g_computingDevice);
		}
	} // namespace ml
} // namespace forge