#pragma once
#include <vector>
#include <string>
#include <random>
#include <filesystem>

class U3Utilities
{
public:
	explicit U3Utilities();
	std::vector<std::string> splitString(const std::string& str, char delimiter, bool keepDelim = false);
	short Absolute(short value);
	int getRandom(int min, int max);
	void trim(std::string& str);
	std::string PathToSDLString(std::filesystem::path input);
	std::string GetPaddedNum(int num, int length);
	std::string replaceAll(std::string str, const std::string& from, const std::string& to);

private:
	std::random_device m_rd;
	std::mt19937 m_rGen;
};

