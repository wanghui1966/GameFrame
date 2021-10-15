#include "redis_manager.h"

#include <stdarg.h>

#include "../debug.h"
#include "../scope_guard.h"

RedisReply::RedisReply(redisReply *redis_reply)
{
	SetRedisReply(redis_reply);
}

RedisReply::~RedisReply()
{
	SetRedisReply(nullptr);
}

void RedisReply::SetRedisReply(redisReply *redis_reply)
{
	if (m_redis_reply)
	{
		freeReplyObject(m_redis_reply);
	}
	m_redis_reply = redis_reply;
}

redisReply* RedisReply::GetReply() const
{
	return m_redis_reply;
}

bool RedisReply::IsArray() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_ARRAY && m_redis_reply->elements > 0);
}

bool RedisReply::IsString() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_STRING && m_redis_reply->len > 0);
}

bool RedisReply::IsInteger() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_INTEGER);
}

bool RedisReply::IsNil() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_NIL);
}

bool RedisReply::IsStatus() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_STATUS);
}

bool RedisReply::IsError() const
{
	return (m_redis_reply && m_redis_reply->type == REDIS_REPLY_ERROR);
}

int RedisReply::GetType() const
{
	if (!m_redis_reply)
	{
		return REDIS_ERR;
	}

	return m_redis_reply->type;
}

bool RedisReply::GetArrayStrings(std::vector<std::string> &datas) const
{
	if (!IsArray())
	{
		return false;
	}

	for (size_t i = 0; i < m_redis_reply->elements; ++i)
	{
		redisReply *r = *(m_redis_reply->element + i);
		if (!r || r->type != REDIS_REPLY_STRING || r->len <= 0)
		{
			datas.emplace_back("");
			continue;
		}
		datas.emplace_back(r->str);
	}
	return true;
}

bool RedisReply::GetString(std::string &data) const
{
	if (!IsString())
	{
		return false;
	}

	data = m_redis_reply->str;
	return true;
}

bool RedisReply::GetInteger(long long &data) const
{
	if (!IsInteger())
	{
		return false;
	}

	data = m_redis_reply->integer;
	return true;
}

bool RedisReply::GetStatus(std::string &data) const
{
	if (!IsStatus())
	{
		return false;
	}

	data = m_redis_reply->str;
	return true;
}

bool RedisReply::GetError(std::string &data) const
{
	if (!IsError())
	{
		return false;
	}

	data = m_redis_reply->str;
	return true;
}

Redis::~Redis()
{
	if (m_connect)
	{
		redisFree(m_connect);
		m_connect = nullptr;
	}
}

bool Redis::Init(const std::string &host, uint32_t port, const std::string &password)
{
	if (host.empty() || port == 0 || password.empty())
	{
		ELOG("Redis::Init fail:host=%s, port=%u, password=%s", host.c_str(), port, password.c_str());
		return false;
	}
	m_host = host;
	m_port = port;
	m_password = password;
	NLOG("Redis::Init success:host=%s, port=%u, password=%s", m_host.c_str(), m_port, m_password.c_str());
	return true;
}

bool Redis::Connect()
{
	m_connect = redisConnect(m_host.c_str(), m_port);
	if (!m_connect)
	{
		ELOG("Redis::Connect fail for nullptr:host=%s, port=%u", m_host.c_str(), m_port);
		return false;
	}
	if (m_connect->err)
	{
		ELOG("Redis::Connect fail for err:host=%s, port=%u, err=%s", m_host.c_str(), m_port, m_connect->errstr);
		redisFree(m_connect);
		m_connect = nullptr;
		return false;
	}

	RedisReply reply;
	if (!ExecuteCommand(reply, "AUTH %s", m_password.c_str()))
	{
		ELOG("Redis::Connect fail for auth:host=%s, port=%u, password=%s", m_host.c_str(), m_port, m_password.c_str());
		redisFree(m_connect);
		m_connect = nullptr;
		return false;
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500000;
	// 设置读/写超时为0.5s
	int timeout_err = redisSetTimeout(m_connect, tv);

	NLOG("Redis::Connect success:host=%s, port=%u, timeout_err=%d", m_host.c_str(), m_port, timeout_err);
	return true;
}

bool Redis::Connect(const std::string &host, uint32_t port, const std::string &password)
{
	if (!Init(host, port, password))
	{
		return false;
	}

	return Connect();
}

bool Redis::ReConnect()
{
	if (redisReconnect(m_connect) != REDIS_OK)
	{
		ELOG("Redis::ReConnect fail:host=%s, port=%u", m_host.c_str(), m_port);
		return false;
	}

	RedisReply reply;
	if (!ExecuteCommand(reply, "AUTH %s", m_password.c_str()))
	{
		ELOG("Redis::ReConnect fail for auth:host=%s, port=%u, password=%s", m_host.c_str(), m_port, m_password.c_str());
		redisFree(m_connect);
		m_connect = nullptr;
		return false;
	}

	NLOG("Redis::ReConnect success:host=%s, port=%u", m_host.c_str(), m_port);
	return true;
}

bool Redis::EnsureConnect()
{
	if (!m_connect)
	{
		return Connect();
	}

	if (m_connect->err != REDIS_OK)
	{
		return ReConnect();
	}

	return true;
}

bool Redis::ExecuteCommand(RedisReply *reply, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	bool ret = ExecuteCommandV(reply, format, ap);
	va_end(ap);
	return ret;
}

bool Redis::ExecuteCommand(RedisReply &reply, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	bool ret = ExecuteCommandV(&reply, format, ap);
	va_end(ap);
	return ret;
}

bool Redis::ExecuteCommandArgv(RedisReply *reply, int argc, const char **argv, const size_t *argvlen)
{
	if (!reply)
	{
		return false;
	}

	if (!EnsureConnect())
	{
		return false;
	}

	redisReply *r = reinterpret_cast<redisReply*>(redisCommandArgv(m_connect, argc, argv, argvlen));
	if (!r)
	{
		ELOG("Redis::ExecuteCommandArgv fail for redisCommandArgv:err_str=%s", m_connect->errstr);
		return false;
	}
	reply->SetRedisReply(r);

	return true;
}

bool Redis::ExecuteCommandArgv(RedisReply &reply, int argc, const char **argv, const size_t *argvlen)
{
	return ExecuteCommandArgv(&reply, argc, argv, argvlen);
}

bool Redis::ExecuteAppendCommand(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	bool ret = true;
	do
	{
		if (!EnsureConnect())
		{
			ret = false;
			break;
		}

		if (redisvAppendCommand(m_connect, format, ap) == REDIS_ERR)
		{
			ELOG("Redis::ExecuteAppendCommand fail for redisvAppendCommand:err_str=%s", m_connect->errstr);
			ret = false;
			break;
		}
	} while (false);
	va_end(ap);

	return ret;
}

bool Redis::ExecuteAppendCommandArgv(int argc, const char **argv, const size_t *argvlen)
{
	if (!EnsureConnect())
	{
		return false;
	}

	if (redisAppendCommandArgv(m_connect, argc, argv, argvlen) == REDIS_ERR)
	{
		ELOG("Redis::ExecuteAppendCommandArgvfail for redisAppendCommandArgv:err_str=%s", m_connect->errstr);
		return false;
	}

	return true;
}

bool Redis::ExecuteCommandV(RedisReply *reply, const char *format, va_list ap)
{
	if (!reply)
	{
		return false;
	}

	if (!EnsureConnect())
	{
		return false;
	}

	redisReply *r = reinterpret_cast<redisReply*>(redisvCommand(m_connect, format, ap));
	if (!r)
	{
		ELOG("Redis::ExecuteCommandV fail for redisvCommand:err_str=%s", m_connect->errstr);
		return false;
	}
	reply->SetRedisReply(r);

	return true;
}

RedisManager::~RedisManager()
{
	std::lock_guard<std::mutex> lck(m_mutex);
	for (auto it = m_pool.begin(); it != m_pool.end(); ++it)
	{
		delete std::get<1>(*it);
	}
	m_pool.clear();
}

bool RedisManager::Init(const std::string &host, uint32_t port, const std::string &password, int pool_init_count/* = 3*/, int pool_max_count/* = 10*/)
{
	if (host.empty() || port == 0 || password.empty() || pool_init_count <= 0 || pool_max_count < pool_init_count)
	{
		ELOG("RedisManager::Init fail:host=%s, port=%u, password=%s, pool_init_count=%d, pool_max_count=%d", host.c_str(), port, password.c_str(), pool_init_count, pool_max_count);
		return false;
	}
	m_host = host;
	m_port = port;
	m_password = password;
	m_pool_init_count = pool_init_count;
	m_pool_max_count = pool_max_count;
	NLOG("RedisManager::Init success:host=%s, port=%u, password=%s, pool_init_count=%d, pool_max_count=%d", m_host.c_str(), m_port, m_password.c_str(), m_pool_init_count, m_pool_max_count);

	return AppendPool(m_pool_init_count);
}

Redis* RedisManager::Get()
{
	Redis *redis = nullptr;

	{
		std::lock_guard<std::mutex> lck(m_mutex);
		for (auto it = m_pool.begin(); it != m_pool.end(); ++it)
		{
			if (std::get<0>(*it))
			{
				continue;
			}

			std::get<0>(*it) = true;
			redis = std::get<1>(*it);
		}
	}

	if (!redis)
	{
		if (AppendPool(m_pool_inc_count))
		{
			return Get();
		}
	}
	return redis;
}

void RedisManager::Put(Redis *redis)
{
	if (!redis)
	{
		ELOG("RedisManager::Put fail for nullptr");
		return;
	}

	std::lock_guard<std::mutex> lck(m_mutex);
	{
		int index = redis->GetIndex();
		int pool_size = m_pool.size();
		if (index <= 0 || index > pool_size)
		{
			ELOG("RedisManager::Put fail for index:index=%d, pool_size=%d", index, pool_size);
			return;
		}

		std::get<0>(m_pool[index - 1]) = false;
	}
}

bool RedisManager::ExecuteCommand(RedisReply *reply, const char *format, ...)
{
	Redis *redis = Get();
	if (!redis)
	{
		return false;
	}
	SCOPE_GUARD([&]{ Put(redis); });

	va_list ap;
	va_start(ap, format);
	bool ret = redis->ExecuteCommand(reply, format, ap);
	va_end(ap);

	return ret;
}

bool RedisManager::ExecuteCommand(RedisReply &reply, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	bool ret = ExecuteCommand(&reply, format, ap);
	va_end(ap);

	return ret;
}

bool RedisManager::AppendPool(int inc_count)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	int success_count = 0;
	for (int i = 0; i < inc_count; ++i)
	{
		if (m_pool.size() >= m_pool_max_count)
		{
			break;
		}

		Redis *redis = new Redis();
		if (!redis->Connect(m_host, m_port, m_password))
		{
			delete redis;
			break;
		}

		redis->SetIndex(m_pool.size() + 1);
		m_pool.emplace_back(std::make_tuple(false, redis));
		++success_count;
	}
	NLOG("RedisManager::AppendPool success:inc_count=%d, success_count=%d, cur_count=%d", inc_count, success_count, (int)m_pool.size());
	return (success_count > 0);
}
