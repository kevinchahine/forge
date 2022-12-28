#include <fstream>
#include <sstream>
#include <chrono>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

#include "forge/ml/StockfishDataset.h"
#include "forge/heuristic/FeatureExtractor.h"
#include "forge/source/globals_torch.h"

#include "forge/time/stopwatch.h"// TODO: remove

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

			// --- Find 1st non space character ---
			size_t fenBegin = line.find_first_not_of(' ');
			if (fenBegin == string::npos) {
				return obj;
			}
			 
			// --- Find 1st (and only) Comma ---
			size_t fenEnd = line.find_first_of(',', fenBegin);
			if (fenEnd == string::npos) {
				return obj;
			}

			// --- Find 1st numeric character ---
			size_t evalBegin = line.find_first_of("-+0123456789", fenEnd);
			if (evalBegin == string::npos) {
				return obj;
			}

			// --- Find last numeric character ---
			size_t evalEnd = line.find_last_of("-+0123456789", evalBegin);
			if (evalEnd == string::npos) {
				return obj;
			}

			// --- Fen ---
			obj.fen = line.substr(fenBegin, fenEnd);

			// --- Eval ---
			stringstream ss(line.substr(evalBegin, evalEnd));
			ss >> obj.eval;

			//boost::regex regex =
			//	//boost::regex(R"dil(\s*([KkQqRrBbNnPp\d\/]+\s+[BbWw]\s+[KQkq-]+\s+[-abcdefgh\d]+\s+\d+\s+\d+)\s*,\s*#{0,1}\s*([-+0123456789]+))dil");
			//	boost::regex(R"dil(\s*(.*)\s*,\s*(.*))dil");
			//
			//boost::sregex_token_iterator regex_it =
			//	boost::sregex_token_iterator(line.begin(), line.end(), regex, { 1, 2 });
			//
			//boost::sregex_token_iterator end;
			//
			//if (regex_it != end) {
			//	obj.fen = *regex_it++;
			//}
			//else {
			//	obj.fen.clear();
			//	return obj;
			//}
			//
			//if (regex_it != end) {
			//	stringstream ss(*regex_it++);
			//	ss >> obj.eval;
			//}
			//else {
			//	obj.fen.clear();
			//	return obj;
			//}

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

			if (_batch.nSamples() == 0) {
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

			//chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
			//
			//const int SZ = 1024 * 1024;
			//
			//vector<char> buff(SZ);
			//
			//ifstream inFile;
			//inFile.open(_csvFile);
			//
			//size_t count = 0;
			//
			//while (int cc = fileRead(inFile, buff)) {
			//	count += countLines(buff, cc);
			//}
			//
			//chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();
			//
			//cout << "Counting lines took " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() 
			//	<< " milliseconds" << endl;
			//
			//_nSamples = count;

			_nSamples = 13'000'000;
		}

		void printTime(const string& name, const StopWatch& sw, const StopWatch& total) {
			cout << name << ": "
				<< chrono::duration_cast<chrono::microseconds>(sw.elapsed()).count() << " usec"
				<< setw(15) << 100.0f * (float) sw.elapsed().count() / (float) total.elapsed().count() << " %"
				<< endl;
		}

		void printTime(const string& name, const chrono::nanoseconds& sw, const chrono::nanoseconds& total) {
			cout << name << ": "
				<< chrono::duration_cast<chrono::microseconds>(sw).count() << " usec"
				<< setw(15) << 100.0f * (float) sw.count() / (float) total.count() << " %"
				<< endl;
		}

		void StockfishDataset::load() {
			StopWatch loadTime;
			StopWatch resizeTime;
			StopWatch getlineTime;
			StopWatch parseTime;
			StopWatch fenTime;
			StopWatch extractTime;
			StopWatch toDeviceTime;

			loadTime.reset();
			loadTime.resume();

			// --- Reset Tensors ---
			resizeTime.resume();
			_batch.resize(_batchSize, forge::heuristic::FeatureExtractor::MATERIAL_FEATURES_SIZE, 1, torch::kCPU);
			_batchIt = 0;
			resizeTime.pause();
			
			// --- Parse Lines ---
			string line;
			size_t count = 0;

			getlineTime.resume();
			while (getline(_in, line) && count < _batchSize) {
				getlineTime.pause();

				try {
					// --- Parse Line info FEN and int ---
					parseTime.resume();
					FenEval fe = FenEval::parse(line);
					parseTime.pause();

					if (fe.isInValid()) {
						stringstream ss;
						ss << "Line could not be parsed: " << line;
						throw std::runtime_error(ss.str());
					}

					// --- Convert FEN to Position ---
					fenTime.resume();
					Position pos;
					pos.fromFEN(fe.fen);
					fenTime.pause();

					// --- Convert To Tensors ---

					// -- Input Tensor --
					extractTime.resume();
					forge::heuristic::FeatureExtractor extractor;
					extractor.init(pos, forge::WHITE);// Extract features from whites perspective
					// *** Whites on the bottom moving up ***
					torch::Tensor sampleSlice = _batch.input.slice(0, count, count + 1);
					extractor.extractMaterial(sampleSlice);
					extractTime.pause();

					// -- Output Tensor --
					_batch.output[count] = fe.eval;
				} catch (const std::exception& e) {
					cout << "Exception _in " << __FILE__ << " line " << __LINE__ << ": " << e.what() << endl
						<< '\t' << line << endl;
					//cin.get();
				}

				count++;

				getlineTime.resume();
			}

			// --- Move Batch to Training Device ---
			toDeviceTime.resume();
			_batch.to(forge::g_computingDevice);
			toDeviceTime.pause();

			chrono::nanoseconds measured =
				resizeTime.elapsed() +
				getlineTime.elapsed() +
				parseTime.elapsed() +
				fenTime.elapsed() +
				extractTime.elapsed() +
				toDeviceTime.elapsed();

			cout << "Execution time from load():" << endl;
			printTime("resize:      ", resizeTime, loadTime);
			printTime("getline:     ", getlineTime, loadTime);
			printTime("parse:       ", parseTime, loadTime);
			printTime("fen:         ", fenTime, loadTime);
			printTime("extract:     ", extractTime, loadTime);
			printTime("todevice:    ", toDeviceTime, loadTime);
			printTime("measured:    ", measured, loadTime.elapsed());
			printTime("total:       ", loadTime, loadTime);
			cout << endl;
			cin.get();
		}
	} // namespace ml
} // namespace forge