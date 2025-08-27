#include <cstdint>
#include <cstdlib>

#include "./mathematics/integer.hpp"
#include "./verify.hpp"

int main()
{
	// Usage example:
	// Integer i{ -17 }; // {} or ()
	// Integer j{ "13" };
	// Integer k = std::string("+11");
	// std::cout << (i * j) % k << '\n'; // output: -1

	// Verify the library
	Verify();

	return EXIT_SUCCESS;
};