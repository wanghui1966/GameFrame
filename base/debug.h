#pragma once

#ifndef DEBUG_H
#define DEBUG_H


// 调试日志接口，可以使接入log系统时不用改任何日志既可以使用

#include "common.h"

#include <ctime>
#include <cstdio>
#include <stdarg.h>

void DebugLog(const char *fmt, ...)
{
	char buf[4096] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	printf("%s %s\n", GetTimeStr(time(nullptr)).c_str(), buf);
	fflush(stdout);
}
#define FLOG(...) DebugLog(__VA_ARGS__)
#define ELOG(...) DebugLog(__VA_ARGS__)
#define WLOG(...) DebugLog(__VA_ARGS__)
#define NLOG(...) DebugLog(__VA_ARGS__)
#define DLOG(...) DebugLog(__VA_ARGS__)

#endif
