#include <misc.h>

std::string toLowerCase(std::string input)
{
	std::transform(input.begin(), input.end(), input.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return input;
}