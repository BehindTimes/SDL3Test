#include <sstream>

#include "U3Utilities.h"


U3Utilities::U3Utilities()
{
	m_rGen.seed(m_rd());
}

std::string U3Utilities::replaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}

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

short U3Utilities::Absolute(short value) // $7E0D
{
	if (value > 127)
	{
		value = (255 - value) + 1;
	}
	if (value < 0)
	{
		value = (-value);
	}
	return value;
}

int U3Utilities::getRandom(int min, int max)
{
	int ret;
	std::uniform_int_distribution<> distrib(min, max);
	ret = distrib(m_rGen);
	return ret;
}

void U3Utilities::trim(std::string& str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}));
}

// SDL was having issues opening up files
std::string U3Utilities::PathToSDLString(std::filesystem::path input)
{
	std::u8string temp = input.u8string();
	std::string strRet(temp.cbegin(), temp.cend());

	return strRet;
}

std::string U3Utilities::GetPaddedNum(int num, int length)
{
	std::stringstream ss;
	ss << std::setw(length) << std::setfill('0') << num;
	return ss.str();
}