#pragma once

#include <boost/asio/streambuf.hpp>
#include <boost/process/pipe.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/asio/io_context.hpp>

#include <iostream>

namespace forge
{
	namespace uci
	{
		class UciBase
		{
		public:
			//UciBase(/*std::istream& in = std::cin, std::ostream& out = std::cout*/) /*:
			//	/*in(in),
			//	out(out)*/ {}

			UciBase() :
				in(&streambufIn),
				apIn(context),
				streambuf_mbt_in(streambufIn.prepare(512))
				{}
			
		protected:
		public:	// TODO: <-- REMOVE THIS
			boost::asio::io_context context;

			boost::process::opstream pout;
			//boost::process::async_pipe apOut;
			//boost::asio::streambuf sBufOut;
			//boost::asio::streambuf::mutable_buffers_type bufsOut;

			std::istream in;
			//std::ostream out;
			//boost::process::ipstream pin;
			boost::process::async_pipe apIn;
			boost::asio::streambuf streambufIn;
			boost::asio::streambuf::mutable_buffers_type streambuf_mbt_in;
		};
	} // namespace uci
} // namespace forge