#pragma once

#ifndef REDIS_SCRIPT_MANAGER_H
#define REDIS_SCRIPT_MANAGER_H

#include <string>
#include <map>

#include "../singleton.h"
#include "redis_manager.h"

// script类型
enum REDIS_SCRIPT_TYPE
{
	RST_NONE							= 0,

	// 字符串类型
	RST_STRING_SET						= 1,
	RST_STRING_GET						= 2,
};

// 是否使用evalsha，否则使用eval
#define USE_EVALSHA 1

#ifdef USE_EVALSHA
	#define SCRIPT_COMMAND "evalsha"
#else
	#define SCRIPT_COMMAND "eval"
#endif

// reids script的管理器
// 使用此类前，须先初始化RedisManager！！！
class RedisScriptManager : public Singleton<RedisScriptManager>
{
public:
	RedisScriptManager() {}
	virtual ~RedisScriptManager() {}

public:
	bool RegisterScript();
	bool GetScriptStrByScriptType(REDIS_SCRIPT_TYPE script_type, std::string &command);

public:
	bool StringSet(const std::string &key, const std::string &value);
	bool StringGet(const std::string &key, std::string &value);

protected:
	// script类型对应的<key=脚本，value=脚本标识>
	std::map<REDIS_SCRIPT_TYPE, std::pair<std::string, std::string>> m_redis_script
	{
		{
			RST_STRING_SET, 
			{
				"redis.call('set', KEYS[1], ARGV[1]) \n"
				"return 0\n", ""
			}
		},
		{
			RST_STRING_GET, 
			{
				"return redis.call('get', KEYS[1])", ""
			}
		},
	};
};
#define sRedisScriptManager (*RedisScriptManager::Instance())

#endif
