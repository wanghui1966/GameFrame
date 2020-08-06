#pragma once

#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <set>

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

	template<typename T>
	static void GetVectorStr(const std::vector<T> &arr, std::string &str)
	{
		if (arr.empty())
		{
			return;
		}

		std::stringstream ss;
		ss << "<";
		for (int i = 0; i < arr.size(); ++i)
		{
			if (i > 0)
			{
				ss << ", ";
			}
			ss << arr[i];
		}
		ss << ">";

		str = ss.str();
	}

	template<typename T>
	static void GetSetStr(const std::set<T> &arr, std::string &str)
	{
		if (arr.empty())
		{
			return;
		}

		std::stringstream ss;
		ss << "<";
		for (auto it = arr.begin(); it != arr.end(); ++it)
		{
			if (it != arr.begin())
			{
				ss << ", ";
			}
			ss << *it;
		}
		ss << ">";

		str = ss.str();
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
