### Introduction

Basic arbitrary integer header only library. With focus on readability, not speed. Comes with a file (verify.hpp) to test the correctness of the library.

##### Input:
Accepts; signed and unsigned (C++) integers and strings.

Not accepted; real/floating point numbers, decimal points, and engineering notation.

##### Usage:
    #include "./mathematics/integer.hpp"

	Integer i{ -17 }; // {} or ()
	Integer j{ "13" };
	Integer k = std::string("+11");
	std::cout << (i * j) % k << '\n'; // output: -1

### Dependencies

- C++11
