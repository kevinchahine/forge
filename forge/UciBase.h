#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/process/pipe.hpp>
#include <boost/process/async_pipe.hpp>

#include <iostream>
#include <sstream>

namespace forge
{
	namespace uci
	{
		class UciBase
		{
		public:
			UciBase() :
				pin(ioc),
				buf_in(512)	// a line will unlikely contain more than 512 char before it is extracted
			{};

			// Extracts a line of text from stream (or async_pipe) `pin` and returns
			// it as a stringstream.
			// This method can be either blocking or non-blocking depending
			// on the text in `pin`.
			// If a full line of text ending with '\n' is in `pin`,
			// then this method is non-blocking.
			// To ensure a non-blocking call to this method, only call when a '\n' is in the stream
			// for example during a call back to async_read_until(...) where the terminal character 
			//	is a '\n'
			std::stringstream readLine();

		protected:
			boost::asio::io_context ioc;

		public:
			boost::process::opstream pout;
			boost::process::async_pipe pin;
			boost::asio::streambuf buf_in;
		};
	} // namespace uci
} // namespace forge