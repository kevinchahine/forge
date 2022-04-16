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

	vector<PositionEvalPair> CSVParser::getNextBatch()
	{
		// --- 1.) Read a batch from csv file ---

		stringstream ss;
		for (size_t nLines = 0; nLines < m_batchSize; nLines++) {
			string line;

			getline(m_inFile, line);

			if (line.empty())	break;
			else				ss << line << endl;
		}

		// --- 2.) Parse batch using csv parser ---
		m_csvDoc = rapidcsv::Document(ss);

		// --- 3.) Convert to Positions and Evals ---
		vector<PositionEvalPair> pairs;
		pairs.reserve(m_batchSize);

		for (size_t rowIndex = 0; rowIndex < m_csvDoc.GetRowCount(); rowIndex++) {
			vector<string> row = m_csvDoc.GetRow<string>(rowIndex);

			const string & fen = row[0];
			heuristic_t eval = stoi(row[1]);

			cout << row[0] << "\t\t\t" << row[1] << endl;
			
			Position pos;
			pos.fromFEN(fen);

			pairs.emplace_back(pos, eval);
		}
		
		return pairs;
	}
} // namespace forge