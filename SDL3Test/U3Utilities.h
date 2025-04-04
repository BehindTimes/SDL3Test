#pragma once
#include <vector>
#include <string>

class U3Utilities
{
public:
	std::vector<std::string> splitString(const std::string& str, char delimiter, bool keepDelim = false);
};

