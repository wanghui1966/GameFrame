#pragma once

#ifndef REDIS_EVALSHA_MANAGER_H
#define REDIS_EVALSHA_MANAGER_H

#include <string>
#include <map>

#include "../singleton.h"
#include "redis_manager.h"

// redis evalsha的key
enum REDIS_EVALSHA_KEY_TYPE
{
	REKT_NONE					= 0,
	REKT_LOCK					= 1,
	REKT_UNLOCK					= 2,
};
static std::map<int, std::string> REDIS_EVALSHA_KEY = 
{
	{ REDIS_LOCK, "redis_lock" },						// redis锁，上锁
	{ REDIS_UNLOCK, "redis_unlock" },					// redis锁，上锁
};
static bool GetRedisEvalshaKey(REDIS_EVALSHA_KEY_TYPE key_type, int64_t id, std::string &key)
{
	if (id <= 0)
	{
		return false;
	}

	auto it = REDIS_EVALSHA_KEY.find(key_type);
	if (it == REDIS_EVALSHA_KEY.end())
	{
		return false;
	}

	key = it->second + "_" + std::to_string(id);
}

// reids evalsha的管理器
// 使用此类前，须先初始化RedisManager！！！
class RedisEvalshaManager : public Singleton<RedisEvalshaManager>
{
public:
	RedisEvalshaManager() {}
	virtual ~RedisEvalshaManager() {}

public:
	bool RegisterEvalshaScript()
	{
		std::string evalsha_key;
		int ret = ExecuteCommandReturnString(evalsha_key);
		if (ret != 0)
		{
			return false;
		}

		NLOG("RedisEvalshaManager::RegisterEvalshaScript success:evalsha_key=%s", evalsha_key.c_str());
		return true;
	}
};
#define sRedisEvalshaManager (*RedisEvalshaManager::Instance())

#endif
