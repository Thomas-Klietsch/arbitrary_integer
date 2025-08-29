// Copyright (c) 2025 Thomas Klietsch, all rights reserved.
//
// Licensed under the GNU Lesser General Public License, version 3.0 or later
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or ( at your option ) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General
// Public License along with this program.If not, see < https://www.gnu.org/licenses/>. 

#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>

// Arbitrary integer
class Integer final
{
	// ASCII (hex) for numbers: 0(30), 1(31), ... 9(39)
	// Convert ASCII number to value: char - '0' = char - 0x30

private:

	// Number is positive, includes zero
	bool f_positive{ true };
	// Stored in reverse order, digit 1 is index zero
	// 32001 stored as 10023
	std::string digit{ "0" };

	// Internal constructor, with no digits.
	// "Safer usage", easier to spot in code, than digit="";
	enum class State { Empty };
	explicit Integer(State)
	{
		digit = "";
	};

	// Remove zeroes from the front of numbers,
	// Front is to the right, i.e. end of string
	void TrimZeroes(
		std::string& value
	) const
	{
		std::uint64_t const index = value.find_last_not_of('0') + 1;
		value.erase(index, std::string::npos);
		// If string is all zeroes, set to zero
		if (value.size() == 0)
			value = "0";
	};

	// Reverse the order of characters in a string.
	void Reverse(
		std::string& value
	) const
	{
		std::reverse(value.begin(), value.end());
	};

public:

	// Default value of (positive) zero (0).
	Integer() = default;

	// Constructor using string
	Integer(std::string const& value)
	{
		set_value(value);
	};

	// Constructor using _only_ integers
	template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
	Integer(T value)
	{
		set_value(std::to_string(value));
	};

	// Unary minus, e.g. change sign
	Integer operator -() const
	{
		Integer result{ *this };
		// Only change sign on non zero value
		if (!result.is_zero())
			result.f_positive = !f_positive;
		return result;
	};

	// Addition
	Integer operator + (Integer const& value) const
	{
		if (is_zero())
			return value;
		if (value.is_zero())
			return *this;

		// Handle differently signed cases
		if (is_positive() && !value.is_positive())
			return Subtract(*this, abs(value));
		if (!is_positive() && value.is_positive())
			return -Subtract(abs(*this), value);

		return Add(*this, value);
	};

	// Subtraction
	Integer operator - (Integer const& value) const
	{
		if (is_zero())
			return -value;
		if (value.is_zero())
			return *this;

		// Handle differently signed cases
		if (is_positive() && !value.is_positive())
			return Add(*this, abs(value));
		if (!is_positive() && value.is_positive())
			return -Add(abs(*this), value);

		return Subtract(*this, value);
	};

	// Multiplication
	Integer operator * (Integer const& value)
	{
		if (is_zero() || value.is_zero())
			return Integer::Zero;
		if (digit == "1")
			return (f_positive
			? value
			: -value);
		if (value.digit == "1")
			return (value.f_positive
			? *this
			: -(*this));

		Integer result;

		std::uint64_t const digits_left = digit.size();
		std::uint64_t const digits_right = value.digit.size();

		// Adds a zero (0) per increase in magnitude.
		std::string digit_padding{ "" };

		for (std::uint64_t i{ 0 };i < digits_left;++i)
		{
			std::uint8_t carry{ 0 };
			Integer temp(State::Empty);
			temp.digit = digit_padding;

			// Character to number [0;9].
			std::uint8_t const a = digit[i] - '0';

			for (std::uint64_t j{ 0 };j < digits_right;++j)
			{
				std::uint8_t const b = value.digit[j] - '0';
				std::uint8_t const c = a * b + carry;
				carry = (c - (c % 10)) / 10;
				// Single digit number to character, and append to string.
				temp.digit += '0' + (c % 10);
			}
			if (carry)
				temp.digit += '0' + carry;

			result = result + temp;
			digit_padding += '0';
		}

		TrimZeroes(result.digit);

		// Set sign
		result.f_positive = f_positive == value.f_positive
			? true
			: false;

		return result;
	};

	// Division
	Integer operator / (Integer const& value) const
	{
		if (value.is_zero())
			throw std::invalid_argument("Divide by zero (number / zero).\n");

		return Divide(*this, value);
	};

	// Modulus, remainder after division
	Integer operator % (Integer const& value) const
	{
		if (value.is_zero())
			throw std::invalid_argument("Modulus with zero (number % zero).\n");

		Integer left{ abs(*this) };
		Integer right{ abs(value) };
		Integer result = left - right * Divide(left, right);
		if (!result.is_zero())
			result.f_positive = f_positive;
		return result;
	};

	// Assignment.

	void operator += (Integer const& value)
	{
		*this = *this + value;
	};

	void operator -= (Integer const& value)
	{
		*this = *this - value;
	};

	void operator *= (Integer const& value)
	{
		*this = *this * value;
	};
	
	void operator /= (Integer const& value)
	{
		*this = *this / value;
	};

	// Logic compare.

	bool operator == (Integer const& value) const
	{
		if (f_positive != value.f_positive)
			return false;
		if (digit.size() != value.digit.size())
			return false;
		for (std::uint64_t i{ 0 }; i < digit.size();++i)
			if (digit[i] != value.digit[i])
				return false;

		return true;
	};

	bool operator != (Integer const& value) const
	{
		return !(*this == value);
	};

	bool operator > (Integer const& value) const
	{
		if (*this == value)
			return false;
		return GreaterThan(*this, value);
	};

	bool operator >= (Integer const& value) const
	{
		if (*this == value)
			return true;
		return GreaterThan(*this, value);
	};

	bool operator < (Integer const& value) const
	{
		if (*this == value)
			return false;
		return GreaterThan(value, *this);
	};

	bool operator <= (Integer const& value) const
	{
		if (*this == value)
			return true;
		return GreaterThan(value, *this);
	};

	// Functions

	// Returns the positive value
	Integer abs(Integer const& value) const
	{
		Integer result{ value };
		result.f_positive = true;
		return result;
	};

	// Usage: variable.abs()
	Integer abs() const
	{
		Integer result{ *this };
		result.f_positive = true;
		return result;
	};

	// Define abs for external access
	friend Integer abs(Integer value)
	{
		value.f_positive = true;
		return value;
	};

	// Checks if Integer is zero (0)
	bool is_zero() const
	{
		if ((digit.size() == 1) && (digit[0] == '0'))
			return true;
		return false;
	};

	// Checks if Integer is positive signed, includes zero (0)
	bool is_positive() const
	{
		return f_positive;
	};

	// Output the Integer
	friend std::ostream& operator <<(std::ostream& os, Integer const& value)
	{
		if (!value.f_positive)
			os << std::string("-");

		std::string result = value.digit;
		std::reverse(result.begin(), result.end());
		os << result;
		return os;
	};

	// Useful constants, final definition after class.
	Integer static const Zero;
	Integer static const One;

private:

	// Internal methods and functions

	void set_value(
		std::string const& value
	)
	{
		// If string is empty
		if (value.size() == 0)
			throw std::invalid_argument("Empty string in constructor.\n");

		// Possible number of characters to skip 
		std::uint64_t skip{ 0 };

		// If string is signed:
		// Set sign, and skip in conversion of number to string
		if (value[0] == '+')
		{
			f_positive = true;
			++skip;
		}
		else if (value[0] == '-')
		{
			f_positive = false;
			++skip;
		}

		// If string is signed, but is otherwise empty
		if (skip && (value.size() == 1))
			throw std::invalid_argument("Empty (signed) string in constructor.\n");

		// Skip leading zeroes
		while (skip < value.size() && value[skip] == '0')
			++skip;

		// Check for non digits in the string (including decimal point)
		for (std::uint64_t i{ skip };i < value.size();++i)
			if (!std::isdigit(value[i]))
				throw std::invalid_argument("Invalid character in string constructor.\n");

		// String is (signed) zeroes
		if (skip == value.size())
		{
			f_positive = true;
			digit = '0';
			return;
		}

		// String is a valid (signed) number, copy numbers.
		// Excludes any sign and leading zeroes.
		digit = value.substr(skip);

		// Reverse string, such that index zero is the single digit
		// e.g. 50321 -> 12305
		Reverse(digit);
	};

	Integer Add(
		Integer const& left,
		Integer const& right
	) const
	{
		Integer result(State::Empty);

		std::uint64_t const digits_left = left.digit.size();
		std::uint64_t const digits_right = right.digit.size();

		std::uint64_t const max_digits = std::max(digits_left, digits_right);

		std::uint8_t carry{ 0 };

		for (std::uint64_t i{ 0 };i < max_digits;++i)
		{
			// Convert character to number, if within string. Otherwise zero
			std::uint8_t const a = i < digits_left ? left.digit[i] - '0' : 0;
			std::uint8_t const b = i < digits_right ? right.digit[i] - '0' : 0;
			std::uint8_t const c = a + b + carry;
			carry = c > 9 ? 1 : 0;
			// Convert number to character, and append to string
			result.digit += '0' + (c % 10);
		}
		if (carry)
			result.digit += '1';

		result.f_positive = left.f_positive;
		return result;
	};

	Integer Subtract(
		// Not referenced, as local copies are needed
		Integer left,
		Integer right
	) const
	{
		Integer result(State::Empty);

		// Handle it as a difference between two numbers,
		// subtracting the small number from the larger,
		// and setting the sign
		if (abs(left) < abs(right))
		{
			result.f_positive = !right.f_positive;
			std::swap(left, right);
		}
		else
			result.f_positive = left.f_positive;

		std::uint64_t const digits_left = left.digit.size();
		std::uint64_t const digits_right = right.digit.size();

		std::uint64_t const max_digits = std::max(digits_left, digits_right);

		std::int8_t carry{ 0 };

		for (std::uint64_t i{ 0 };i < max_digits;++i)
		{
			std::uint8_t const a = i < digits_left ? left.digit[i] - '0' : 0;
			std::uint8_t const b = i < digits_right ? right.digit[i] - '0' : 0;
			std::int8_t c = a - b - carry;
			carry = c < 0 ? 1 : 0;
			if (c < 0)
				c += 10;

			result.digit += '0' + (c % 10);
		}
		if (carry)
			result.f_positive = false;

		TrimZeroes(result.digit);

		// Catch negative zero (-0)
		if (result.is_zero())
			result.f_positive = true;

		return result;
	};

	Integer Divide(
		Integer const& left,
		Integer const& right
	) const
	{
		if (left.is_zero() || right.is_zero())
			return Integer::Zero; // TODO add NaN/inf support?

		if (abs(right) > abs(left))
			return Integer::Zero; // result is less than 1

		if (abs(left) == abs(right))
			return (left.is_positive() == right.is_positive()
			? Integer::One
			: -Integer::One);

		Integer result(State::Empty);

		std::uint64_t const digits_left = left.digit.size();
		std::uint64_t const digits_right = right.digit.size();

		// Create a nominator with the same magnitude as denominator (right)
		Integer nominator;
		nominator.digit = digit.substr(digits_left - digits_right, digits_left);
		Integer const denominator{ abs(right) };

		std::uint64_t index = digits_left - digits_right + 1;
		std::uint8_t count{ 0 };
		while (index > 0)
		{
			if (nominator < denominator)
			{
				// Divide starts with MSB (largest digits).
				// But writes in reverse order (fewer memory operations)
				result.digit += '0' + count;
				count = 0;
				--index;
				// Shift in new digit
				nominator.digit = digit[index - 1] + nominator.digit;
				TrimZeroes(nominator.digit);
			}
			else
			{
				++count;
				nominator = nominator - denominator;
			}
		}

		result.f_positive = left.is_positive() == right.is_positive();
		// See note above, about MSB
		Reverse(result.digit);
		TrimZeroes(result.digit);
		return result;
	};

	bool GreaterThan(
		Integer const& left,
		Integer const& right
	) const
	{
		if (left.f_positive && !right.f_positive)
			return true;
		if (!left.f_positive && right.f_positive)
			return false;

		if (left.f_positive)
		{
			if (left.digit.size() < right.digit.size())
				return false;
			if (left.digit.size() == right.digit.size())
				for (std::uint64_t i{ right.digit.size() }; i > 0;--i)
				{
					std::uint8_t const a = left.digit[i - 1];
					std::uint8_t const b = right.digit[i - 1];
					if (a < b)
						return false;
					if (a > b)
						break;
				}
		}
		else
		{
			if (left.digit.size() > right.digit.size())
				return false;
			if (left.digit.size() == right.digit.size())
				for (std::uint64_t i{ digit.size() }; i > 0;--i)
				{
					std::uint8_t const a = left.digit[i - 1];
					std::uint8_t const b = right.digit[i - 1];
					if (a < b)
						break;
					if (a > b)
						return false;
				}
		}

		return true;
	};

};

// Finalise definition of static constants in Integer class
Integer const Integer::Zero(0);
Integer const Integer::One(1);

// Greatest common (positive) divisor,
// e.g.largest divisor that is possible for both a and b
// Returns GCD of |a| and |b|
Integer GCD(
	Integer a,
	Integer b
)
{
	a = abs(a);
	b = abs(b);

	if (a.is_zero())
		return b;
	else if (b.is_zero())
		return a;

	if (a < b)
		return GCD(a, b % a);
	return GCD(b, a % b);
};

// Returns GCD of all elements in a vector
Integer GCD(
	std::vector<Integer> const& a
)
{
	if (!a.size())
		return Integer::Zero;
	Integer gcd{ abs(a[0]) };
	for (std::uint64_t i{ 0 }; i < a.size(); ++i)
		gcd = GCD(a[i], gcd);
	return gcd;
};

Integer Factorial(
	std::uint8_t const x
)
{
	Integer result{ 1 };
	for (std::uint16_t i{ 1 }; i <= x; ++i)
		result *= i;
	return result;
};

Integer FactorialFalling(
	std::uint8_t const x,
	std::uint8_t const n
)
{
	if (n > x)
		return Integer::Zero;
	Integer result{ x };
	for (std::uint16_t i{ 1 }; i < n; ++i)
		result *= (x - i);
	return result;
};

Integer FactorialRising(
	std::uint8_t const x,
	std::uint8_t const n
)
{
	Integer result{ x };
	for (std::uint16_t i{ 1 }; i < n; ++i)
		result *= (x + i);
	return result;
};

Integer Pow2(
	std::uint8_t const n
)
{
	Integer result{ 1 };
	for (std::uint16_t i{ 0 }; i < n; ++i)
		result *= 2;
	return result;
};
