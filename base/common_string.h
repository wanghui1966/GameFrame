#pragma once

#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <algorithm>
#include <string>
#include <vector>

class StringHelper
{
public:
	static std::vector<std::string> Split(const std::string &str, char sep)
	{
		std::vector<std::string> result;
		if (str.empty())
		{
			return result;
		}
		std::string strstr = str + sep;
		size_t size = strstr.size();

		size_t  pos = 0;
		size_t i = 0;
		while (i < size)
		{
			pos = strstr.find(sep, i);
			if (pos != std::string::npos)
			{
				std::string temp = strstr.substr(i, pos - i);
				if (!temp.empty())
				{
					result.push_back(temp);
				}
				i = pos + 1;
			}
			else
			{
				break;
			}
		}
		return result;
	}

	static std::string ToUpper(const std::string& str)
	{
		std::string sstr(str);
		std::transform(sstr.begin(), sstr.end(), sstr.begin(), [](unsigned char c){ return std::toupper(c); });
		return sstr;
	}

	static std::string ToLower(const std::string& str)
	{
		std::string sstr(str);
		std::transform(sstr.begin(), sstr.end(), sstr.begin(), [](unsigned char c){ return std::tolower(c); });
		return sstr;
	}
};

#endif
