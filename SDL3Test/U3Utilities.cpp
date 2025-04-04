#include "U3Utilities.h"
#include <sstream>

std::vector<std::string> U3Utilities::splitString(const std::string& str, char delimiter, bool keepDelim)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		if (token.size() > 0)
		{
			tokens.push_back(token);
		}
		else if (keepDelim)
		{
			std::string strDelim("");
			strDelim += delimiter;
			tokens.push_back(strDelim);
		}
	}
	return tokens;
}