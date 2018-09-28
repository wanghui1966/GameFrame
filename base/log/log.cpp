#include "log.h"

#include <cstdio>
#include <cstring>
#include <string>

void Log::Init(const std::string &log_properties)
{
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(log_properties.c_str()));

	m_logger_server = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("server"));
}

void Log::Update()
{
	std::string log;
	while (true)
	{
		if (m_log_ts_queue.Empty() || !m_log_ts_queue.Pop(log) || log.size() < 20)
		{
			continue;
		}

		char level_c = log[20];
		switch (level_c)
		{
			case 'D':
				LOG4CPLUS_DEBUG(m_logger_server, log.c_str());
				break;
			case 'N':
				LOG4CPLUS_INFO(m_logger_server, log.c_str());
				break;
			case 'W':
				LOG4CPLUS_WARN(m_logger_server, log.c_str());
				break;
			case 'E':
				LOG4CPLUS_ERROR(m_logger_server, log.c_str());
				break;
			case 'F':
				LOG4CPLUS_FATAL(m_logger_server, log.c_str());
				break;
			default:
				break;
		}
	}
}

void Log::PrintLog(const char *file_name, const char *func_name, int line, char lv_c, const char *fmt, ...)
{
	if (m_log_ts_queue.Size() > SERVER_LOG_TS_QUEUE_MAX_SIZE)
	{
		return;
	}
	std::string log;

	// 日志头
	{
		char buf[64];

		time_t now = time(nullptr);
		struct tm tm_now;
		localtime_r(&now, &tm_now);
		memset(buf, 0, sizeof(buf));
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_now);
		log.append(buf);

		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "|%c|", lv_c); 
		log.append(buf);
	}

	// 日志体
	std::string body;
	{
		char buf[4096];

		memset(buf, 0, sizeof(buf));
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);
		log.append(buf);
	}

	// 日志尾
	std::string tail;
	if (lv_c == 'F' || lv_c == 'E')
	{
		char buf[256];
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), " [[[ file_name=%s, func_name=%s, line=%u ]]]\n", file_name, func_name, line);
		log.append(buf);
	}
	else
	{
		log.append("\n");
	}

	m_log_ts_queue.Push(log);
}
