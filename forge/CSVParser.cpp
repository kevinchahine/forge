#include "CSVParser.h"

#include <sstream>
#include <string>

using namespace std;

namespace forge
{
	void CSVParser::open(const string & csvFilename)
	{
		try {
			m_inFile.open(csvFilename);

			if (m_inFile) {
				cout << csvFilename << " openned :)" << endl;

				this->csvFilename = csvFilename;
			}
			else {
				cout << csvFilename << " somethings wrong :(" << endl;
			}
		}
		catch (exception & e) {
			cout << "Exception: " << e.what() << endl;
		};
	}
	
	void CSVParser::close()
	{
		m_inFile.close();
	}

	bool CSVParser::isOpen() const
	{
		return m_inFile.is_open();
	}

	void CSVParser::reset()
	{
		close();

		open(csvFilename);
	}

	vector<PositionEvalPair> CSVParser::getNextBatch()
	{
		stringstream ss;
		string line;

		// --- 1.) Read a batch from csv file ---

		// Read the 1st line and see if it is the header
		getline(m_inFile, line);
		
		// Is this line the csv header?
		if (line.find("FEN") != string::npos) {
			// Yes. This line is the header. We can ignore it.
		}
		else {
			// No. This is a sample. We should keep it. Just make sure its not empty.
			if (line.empty() == false)
				ss << line << endl;	
		}

		// Read the rest of the lines 		
		for (size_t nLines = 0; nLines < m_batchSize; nLines++) {
			getline(m_inFile, line);

			if (line.empty())	break;
			else				ss << line << endl;
		}

		// --- 2.) Parse the batch using csv parser ---
		m_csvDoc = rapidcsv::Document(ss, rapidcsv::LabelParams(-1, -1));	// Treat 1st line as a sample (not a header)

		// --- 3.) Convert to Positions and Evals ---
		vector<PositionEvalPair> pairs;
		pairs.reserve(m_batchSize);

		for (size_t rowIndex = 0; rowIndex < m_csvDoc.GetRowCount(); rowIndex++) {
			vector<string> row = m_csvDoc.GetRow<string>(rowIndex);

			try {
				// --- Position FEN ---
				const string & fen = row[0];

				Position pos;
				pos.fromFEN(fen);

				// --- Eval ---
				// ignore '#' if it exists (indicates a mate position)
				string evalStr = row[1];

				if (evalStr[0] == '#') {
					evalStr = evalStr.substr(1);	// Skip the 1st char (the '#')
				}

				// Ignore plus

				heuristic_t eval = stoi(evalStr);

				pairs.emplace_back(pos, eval);
			}
			catch (std::exception & e) {
				cout << "Exception in " << __FILE__ << " line " << __LINE__ << ": " << e.what() << endl
					<< "FEN:     \'" << row[0] << '\'' << endl
					<< "evalStr: \'" << row[1] << '\'' << endl;
			};
		}

		return pairs;
	}
} // namespace forge