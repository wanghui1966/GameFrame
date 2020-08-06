#pragma once

#ifndef DEBUG_H
#define DEBUG_H


// 调试日志接口，可以使接入log系统时不用改任何日志既可以使用

#include "common_time.h"

#include <cstdio>
#include <stdarg.h>

static void DebugLog(int log_level, const char *fmt, ...)
{
	if (log_level == 5)
	{
#ifdef __NO_DEBUG_LOG
		return;
#endif
	}

	char buf[4096] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	if (log_level == 1 || log_level == 2)
	{
		printf("%s \033[0;31m%s\033[0m\n", TimeHelper::GetNowTimeStr().c_str(), buf);
	}
	else
	{
		printf("%s %s\n", TimeHelper::GetNowTimeStr().c_str(), buf);
	}
	fflush(stdout);
}

#define FLOG(...) DebugLog(1, __VA_ARGS__)
#define ELOG(...) DebugLog(2, __VA_ARGS__)
#define WLOG(...) DebugLog(3, __VA_ARGS__)
#define NLOG(...) DebugLog(4, __VA_ARGS__)
#define DLOG(...) DebugLog(5, __VA_ARGS__)

#endif
