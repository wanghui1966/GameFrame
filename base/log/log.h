#ifndef LOG_H
#define LOG_H

#include "../singleton.h"
#include "../thread_safe_container/ts_queue.h"

#include "../../third_party_lib/log4cplus/include/log4cplus/logger.h"
#include "../../third_party_lib/log4cplus/include/log4cplus/loggingmacros.h"
#include "../../third_party_lib/log4cplus/include/log4cplus/configurator.h"

#include <string>

// server日志最大缓存数量
const int SERVER_LOG_TS_QUEUE_MAX_SIZE = 1024;

class Log : public Singleton<Log>
{
public:
	Log() {}
	virtual ~Log() {}

public:
	void Init(const std::string &log_properties);

	void Update();

	void PrintLog(const char *cur_file, const char *cur_fun, int cur_line, char lv_c, const char *fmt, ...);

protected:
	log4cplus::Logger m_logger_server;
	TSQueue<std::string> m_log_ts_queue;
};

#define sLog (*Log::Instance())

// fatal
#define FLOG(...) sLog.PrintLog(__FILE__, __FUNCTION__, __LINE__, 'F', __VA_ARGS__)
// error
#define ELOG(...) sLog.PrintLog(__FILE__, __FUNCTION__, __LINE__, 'E', __VA_ARGS__)
// warn
#define WLOG(...) sLog.PrintLog(__FILE__, __FUNCTION__, __LINE__, 'W', __VA_ARGS__)
// info
#define NLOG(...) sLog.PrintLog(__FILE__, __FUNCTION__, __LINE__, 'N', __VA_ARGS__)
// debug
#define DLOG(...) sLog.PrintLog(__FILE__, __FUNCTION__, __LINE__, 'D', __VA_ARGS__)

#endif
