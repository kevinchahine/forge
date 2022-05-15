#pragma once

#include <string>

namespace forge
{
	namespace uci
	{
		class UciGui
		{
		public:

		public:
			void registerEngine(const std::string& name, const std::string& val);

			void registerLater();

			void registerName(const std::string& name);

			void registerCode(const std::string& code);

		};
	} // namespace uci
} // namespace forge