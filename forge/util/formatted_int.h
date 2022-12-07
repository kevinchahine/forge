#pragma once

#include <iostream>
#include <string>
#include <sstream>

namespace forge
{
	class formatted_int
	{
	public:
		formatted_int() = default;
		formatted_int(int value) : m_value(value) {}
		formatted_int(const formatted_int&) = default;
		formatted_int(formatted_int&&) noexcept = default;
		~formatted_int() noexcept = default;
		formatted_int& operator=(const formatted_int&) = default;
		formatted_int& operator=(formatted_int&&) noexcept = default;

		friend std::ostream& operator<<(std::ostream& os, const formatted_int& number) {
			bool negative = (number.m_value < 0);

			std::string digits = std::to_string(abs(number.m_value));
			std::stringstream ss;
			size_t count = 0;

			std::string::reverse_iterator it = digits.rbegin();
			while (it < digits.rend()) {
				if (count % 3 == 0 && count != 0) {
					ss << '\'';
				}

				ss << *it;
				++it;
				++count;
			}

			digits = ss.str();

			if (negative) {
				os << '-';
			}

			it = digits.rbegin();
			while (it != digits.rend()) {
				os << *it;
				++it;
			}

			return os;
		}

	private:
		int m_value = 0;
	};
} // namespace forge