#include "U3Utilities.h"
#include <sstream>

std::vector<std::string> U3Utilities::splitString(const std::string& str, char delimiter, bool keepDelim)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	std::string strDelim("");
	strDelim += delimiter;
	bool init = true;

	if (str.size() > 0)
	{

		while (std::getline(ss, token, delimiter))
		{
			if (init)
			{
				init = false;
			}
			else
			{
				if (keepDelim)
				{
					tokens.push_back(strDelim);
				}
			}
			if (token.size() > 0)
			{
				tokens.push_back(token);
			}
		}
		if (keepDelim)
		{
			if (str.ends_with(delimiter))
			{
				tokens.push_back(strDelim);
			}
		}
	}
	return tokens;
}