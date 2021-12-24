#include "UciBase.h"

using namespace std;

namespace forge
{
	namespace uci
	{
		std::stringstream UciBase::readLine()
		{
			std::istream stream(&buf_in);
			std::string line;

			// Extract 1 full line of text ending with '\n'
			getline(stream, line);

			// Sometimes (usually when run on windows) the line ends with '\r'.
			// Remove this to prevent loosing the 1st character during streaming.
			if (line.back() == '\r') {
				line.pop_back();
			}

			// Move line to a stringstream and return.
			return std::stringstream(move(line));
		}
	} // namespace forge
} // namespace forge