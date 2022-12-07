#pragma once

#include <iostream>

namespace forge {
	// The opposite of an atomic. 
	// Used to represent non-atomic primitive types in a template.
	// All operations performed by this class are the same as they would be performed
	// without the use of atomic instructions. 
	template<typename T>
	class composite
	{
	public:
		composite() = default;
		composite(const composite&) = default;
		composite(composite&&) noexcept = default;
		composite(T value) : _value(value) {}
		~composite() noexcept = default;
		
		// ------------------------ OPERATOR OVERLOADS ------------------------

		composite& operator=(const composite&) = default;
		composite& operator=(composite&&) noexcept = default;
		composite& operator=(T value) { _value = value; return *this; }

		T operator++() { return ++_value; }
		T operator++(int) { return _value++; }

		bool operator<(const T & rhs) const { return this->_value < rhs; }
		bool operator>(const T & rhs) const { return this->_value > rhs; }
		bool operator==(const T & rhs) const { return this->_value == rhs; }
		bool operator!=(const T & rhs) const { return this->_value != rhs; }
		bool operator<=(const T & rhs) const { return this->_value <= rhs; }
		bool operator>=(const T & rhs) const { return this->_value >= rhs; }

		bool operator<(const  composite<T>& rhs) const { return this->_value <  rhs._value; }
		bool operator>(const  composite<T>& rhs) const { return this->_value >  rhs._value; }
		bool operator==(const composite<T>& rhs) const { return this->_value == rhs._value; }
		bool operator!=(const composite<T>& rhs) const { return this->_value != rhs._value; }
		bool operator<=(const composite<T>& rhs) const { return this->_value <= rhs._value; }
		bool operator>=(const composite<T>& rhs) const { return this->_value >= rhs._value; }

		friend std::ostream& operator<<(std::ostream& os, const composite<T>& comp) {
			os << comp.value();

			return os;
		}

		T& value() { return _value; }
		const T& value() const { return _value; }

		T load() { return _value; }
		void store(T value) { _value = value; }

		T exchange(T new_value) { T curr = _value; _value = new_value; return curr; }

		bool compare_exchange_weak(T& expected, T desired) {
			T curr = _value;
			bool result = false;

			if (_value == expected) {
				_value = desired;
				result = true;
			}

			expected = curr;
			return result;
		}

		bool compare_exchange_strong(T& expected, T desired) {
			T curr = _value;
			bool result = false;

			if (_value == expected) {
				_value = desired;
				result = true;
			}

			expected = curr;
			return result;
		}

		// ------------------------- FETCH OPERATIONS --------------------------------

		T fetch_add(T v) {
			T curr = _value;
			_value += v;
			return curr;
		}

		T fetch_sub(T v) {
			T curr = _value;
			_value -= v;
			return curr;
		}

		T fetch_and(T v) {
			T curr = _value;
			_value &= v;
			return curr;
		}

		T fetch_or(T v) {
			T curr = _value;
			_value |= v;
			return curr;
		}

		T fetch_xor(T v) {
			T curr = _value;
			_value ^= v;
			return curr;
		}

		T fetch_negate() {
			T curr = _value;
			_value = -_value;
			return curr;
		}

		// ------------------------- OPERATIONS --------------------------------------

		T add(T v) { return _value += v; }
		T sub(T v) { return _value -= v; }
		T bitwise_and(T v) { return _value &= v; }
		T bitwise_or(T v) { return _value |= v; }
		T bitwise_xor(T v) { return _value ^= v; }

	private:
		T _value;
	};
} // namespace forge

template<typename LEFT_T, typename RIGHT_T> bool operator< (const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs <  rhs._value; }
template<typename LEFT_T, typename RIGHT_T> bool operator> (const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs >  rhs._value; }
template<typename LEFT_T, typename RIGHT_T> bool operator==(const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs == rhs._value; }
template<typename LEFT_T, typename RIGHT_T> bool operator!=(const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs != rhs._value; }
template<typename LEFT_T, typename RIGHT_T> bool operator<=(const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs <= rhs._value; }
template<typename LEFT_T, typename RIGHT_T> bool operator>=(const LEFT_T& lhs, const forge::composite<RIGHT_T> & rhs) { return lhs >= rhs._value; }
