#include "UciBase.h"

using namespace std;

namespace forge
{
	namespace uci
	{
		std::stringstream UciBase::readLine()
		{
			//pthread_error std::istream stream(&buf_in);
			//pthread_error std::string line;
			//pthread_error 
			//pthread_error // Extract 1 full line of text ending with '\n'
			//pthread_error getline(stream, line);
			//pthread_error 
			//pthread_error // Sometimes (usually when run on windows) the line ends with '\r'.
			//pthread_error // Remove this to prevent loosing the 1st character during streaming.
			//pthread_error if (line.back() == '\r') {
			//pthread_error 	line.pop_back();
			//pthread_error }
			//pthread_error 
			// Move line to a stringstream and return.
			string line; //pthread_error 
			return std::stringstream(move(line));
		}
	} // namespace forge
} // namespace forge