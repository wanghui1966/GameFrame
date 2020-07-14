#pragma once

#ifndef COMMON_TIME_H
#define COMMON_TIME_H

#include <ctime>
#include <string>

class TimeHelper
{
public:
	static std::string GetTimeStr(time_t t, bool is_simple = false)
	{
		struct tm time_tm;
		localtime_r(&t, &time_tm);
		char buf[64] = {0};
		if (is_simple)
		{
			strftime(buf, sizeof(buf), "%d %H:%M:%S", &time_tm);
		}
		else
		{
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &time_tm);
		}
		return buf;
	}

	static std::string GetNowTimeStr(bool is_simple = false)
	{
		time_t now_time = time(nullptr);
		return GetTimeStr(now_time, is_simple);
	}

};

#endif
