
#pragma once

// Header file to verify that library match C++
// Call Verify() for full test suit

#include "./mathematics/integer.hpp"

// Logic

bool Verify_Equal()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a == b) != (Integer(a) == Integer(b)))
				return false;
	return true;
};

bool Verify_NotEqual()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a != b) != (Integer(a) != Integer(b)))
				return false;
	return true;
};

bool Verify_Greater()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a > b) != (Integer(a) > Integer(b)))
				return false;
	return true;
};

bool Verify_GreaterEqual()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a >= b) != (Integer(a) >= Integer(b)))
				return false;
	return true;
};

bool Verify_Lesser()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a < b) != (Integer(a) < Integer(b)))
				return false;
	return true;
};

bool Verify_LesserEqual()
{
	for (std::int64_t a{ -100 };a <= 100;++a)
		for (std::int64_t b{ -100 };b <= 100;++b)
			if ((a <= b) != (Integer(a) <= Integer(b)))
				return false;
	return true;
};

// Arithmetic

bool Verify_Add()
{
	for (std::int64_t a{ -1000 };a <= 1000;++a)
		for (std::int64_t b{ -1000 };b <= 1000;++b)
			if (Integer(a + b) != (Integer(a) + Integer(b)))
				return false;
	return true;
};

bool Verify_Subtract()
{
	for (std::int64_t a{ -1000 };a <= 1000;++a)
		for (std::int64_t b{ -1000 };b <= 1000;++b)
			if (Integer(a - b) != (Integer(a) - Integer(b)))
				return false;
	return true;
};

bool Verify_Multiply()
{
	for (std::int64_t a{ -1000 };a <= 1000;++a)
		for (std::int64_t b{ -1000 };b <= 1000;++b)
			if (Integer(a * b) != (Integer(a) * Integer(b)))
				return false;
	return true;
};

bool Verify_Divide()
{
	for (std::int64_t a{ -1000 };a <= 1000;++a)
		for (std::int64_t b{ -1000 };b <= 1000;++b)
			if (b != 0)
				if (Integer(a / b) != (Integer(a) / Integer(b)))
					return false;
	return true;
};

bool Verify_Modulus()
{
	for (std::int64_t a{ -1000 };a <= 1000;++a)
		for (std::int64_t b{ -1000 };b <= 1000;++b)
			if (b != 0)
				if (Integer(a % b) != (Integer(a) % Integer(b)))
					return false;
	return true;
};

void Verify()
{
	// Invalid strings and values
	std::cout << "Catch four (4) invalid Integer contructors:\n";
	try { Integer(""); }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}
	try { Integer("-"); }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}
	try { Integer("1.1f"); }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}
	try { Integer("1e10"); }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}
	std::cout << "Catch two (2) invalid operations:\n";
	try { Integer(10) / Integer::Zero; }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}
	try { Integer("10") % Integer::Zero; }
	catch (std::exception const& e) {
		std::cout << "Exception catch: " << e.what();
	}

	std::cout << "Verify == : " << (Verify_Equal() ? "okay" : "failure") << ".\n";
	std::cout << "Verify != : " << (Verify_NotEqual() ? "okay" : "failure") << ".\n";
	std::cout << "Verify >  : " << (Verify_Greater() ? "okay" : "failure") << ".\n";
	std::cout << "Verify >= : " << (Verify_GreaterEqual() ? "okay" : "failure") << ".\n";
	std::cout << "Verify <  : " << (Verify_Lesser() ? "okay" : "failure") << ".\n";
	std::cout << "Verify <= : " << (Verify_LesserEqual() ? "okay" : "failure") << ".\n";

	std::cout << "Verify +  : " << (Verify_Add() ? "okay" : "failure") << ".\n";
	std::cout << "Verify -  : " << (Verify_Subtract() ? "okay" : "failure") << ".\n";
	std::cout << "Verify *  : " << (Verify_Multiply() ? "okay" : "failure") << ".\n";
	std::cout << "Verify /  : " << (Verify_Divide() ? "okay" : "failure") << ".\n";
	std::cout << "Verify %  : " << (Verify_Modulus() ? "okay" : "failure") << ".\n";

	std::cout << "Verify 2^64 = " << Pow2(64) << '\n';
	std::cout << "Verify 2^64 = 18446744073709551616 (reference)\n";


	std::cout << "Verify 50! = " << Factorial(50) << '\n';
	std::cout << "Verify 50! = 30414093201713378043612608166064768844377641568960512000000000000 (reference)\n";

	std::cout << "Verify GCD(2^64,50!) = " << GCD(Pow2(64), Factorial(50)) << '\n';
	std::cout << "Verify GCD(2^64,50!) = 140737488355328 (reference)\n";
};
