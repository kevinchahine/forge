#pragma once

#include <iostream>
#include <string>
#include <typeinfo>

namespace forge
{
	namespace util
	{
		template<typename T>
		std::string demangle(const T & var) {
			std::string name = typeid(var).name();

			size_t last_space = name.find_last_of(' ') + 1;

			return name.substr(last_space);
		}
	} // namespace util
} // namespace forge