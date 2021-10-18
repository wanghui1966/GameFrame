#include "redis_lock.h"

#include <chrono>
#include <thread>

#include "../snow_flake.h"
#include "redis_manager.h"

RedisLock::RedisLock()
{
	Init();
	NLOG("RedisLock::RedisLock:reids_lock_key=%s", m_reids_lock_key.c_str());
}

RedisLock::RedisLock(const std::string &redis_lock_key)
{
	m_reids_lock_key = redis_lock_key;
	NLOG("RedisLock::RedisLock:reids_lock_key=%s", m_reids_lock_key.c_str());
}

RedisLock::~RedisLock()
{
	NLOG("RedisLock::~RedisLock:reids_lock_key=%s", m_reids_lock_key.c_str());
}

bool RedisLock::TryLock()
{
	RedisReply redis_reply;
	if (sRedisManager.ExecuteCommand(redis_reply, "set %s 1 nx", m_reids_lock_key.c_str()) && redis_reply.IsStatus())
	{
		return true;
	}

	ELOG("RedisLock::TryLockfail for retry_count:reids_lock_key=%s", m_reids_lock_key.c_str());
	return false;
}

bool RedisLock::Lock()
{
	RedisReply redis_reply;
	int retry_count = REDIS_LOCK_RETRY_COUNT;
	while (retry_count)
	{
		if (sRedisManager.ExecuteCommand(redis_reply, "set %s 1 NX", m_reids_lock_key.c_str()) && redis_reply.IsStatus())
		{
			return true;
		}

		--retry_count;
		std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_LOCK_RETRY_INTERVAL));
	}

	ELOG("RedisLock::Lock fail for retry_count:reids_lock_key=%s", m_reids_lock_key.c_str());
	return false;
}

bool RedisLock::Unlock()
{
	int ret = 0;
	do
	{
		RedisReply redis_reply;
		if (!sRedisManager.ExecuteCommand(redis_reply, "del %s", m_reids_lock_key.c_str()))
		{
			ret = -1;
			break;
		}

		long long result = 0;
		if (!redis_reply.GetInteger(result))
		{
			ret = -2;
			break;
		}
		if (result == 0)
		{
			ret = -3;
			break;
		}
	} while (false);
	if (ret != 0)
	{
		ELOG("RedisLock::Unlock fail:reids_lock_key=%s, ret=%d", m_reids_lock_key.c_str(), ret);
	}

	return (ret == 0);
}

std::string RedisLock::GetKey()
{
	return m_reids_lock_key;
}

void RedisLock::Init()
{
	SnowFlake sf;
	sf.Init(LOCAL_DATA_CENTER_ID, LOCAL_MACHINE_ID);
	m_reids_lock_key = std::to_string(sf.GetNextId());
}
