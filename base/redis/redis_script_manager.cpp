#include "redis_script_manager.h"

#include "../debug.h"

bool RedisScriptManager::RegisterScript()
{
#ifdef USE_EVALSHA
	for (auto it = m_redis_script.begin(); it != m_redis_script.end(); ++it)
	{
		RedisReply redis_reply;
		if (!sRedisManager.ExecuteCommand(redis_reply, "script load %s", it->second.first.c_str()))
		{
			ELOG("RedisScriptManager::RegisterScript fail for ExecuteCommand:script_type=%d", it->first);
			continue;
		}
		std::string evalsha_str;
		if (!redis_reply.GetString(evalsha_str))
		{
			ELOG("RedisScriptManager::RegisterScript fail for GetString:script_type=%d", it->first);
			continue;
		}

		it->second.second = evalsha_str;
		NLOG("RedisScriptManager::RegisterScript success:script_type=%d, evalsha_str=%s", it->first, evalsha_str.c_str());
	}
#endif
	return true;
}

bool RedisScriptManager::GetScriptStrByScriptType(REDIS_SCRIPT_TYPE script_type, std::string &command)
{
	auto it = m_redis_script.find(script_type);
	if (it == m_redis_script.end())
	{
		return false;
	}

#ifdef USE_EVALSHA
	command = it->second.second;
#else
	command = it->second.first;
#endif
	return (!command.empty());
}

bool RedisScriptManager::StringSet(const std::string &key, const std::string &value)
{
	int ret = 0;
	do
	{
		std::string command;
		if (!GetScriptStrByScriptType(RST_STRING_SET, command))
		{
			ret = -1;
			break;
		}

		RedisReply redis_reply;
		if (!sRedisManager.ExecuteCommand(redis_reply, "%s %s 1 %s %s", SCRIPT_COMMAND, command.c_str(), key.c_str(), value.c_str()))
		{
			ret = -2;
			break;
		}

		long long ret = 0;
		if (!redis_reply.GetInteger(ret))
		{
			ret = -3;
			break;
		}
		if (ret != 0)
		{
			ret = -4;
			break;
		}
	} while (false);

	if (ret == 0)
	{
		NLOG("RedisScriptManager::StringGet success:key=%s, value=%s", key.c_str(), value.c_str());
	}
	else
	{
		ELOG("RedisScriptManager::StringGet fail:key=%s, ret=%d", key.c_str(), ret);
	}
	return (ret == 0);
}

bool RedisScriptManager::StringGet(const std::string &key, std::string &value)
{
	int ret = 0;
	do
	{
		std::string command;
		if (!GetScriptStrByScriptType(RST_STRING_GET, command))
		{
			ret = -1;
			break;
		}

		RedisReply redis_reply;
		if (!sRedisManager.ExecuteCommand(redis_reply, "%s %s 1 %s", SCRIPT_COMMAND, command.c_str(), key.c_str()))
		{
			ret = -2;
			break;
		}

		if (!redis_reply.GetString(value))
		{
			ret = -3;
			break;
		}
	} while (false);

	if (ret == 0)
	{
		NLOG("RedisScriptManager::StringGet success:key=%s, value=%s", key.c_str(), value.c_str());
	}
	else
	{
		ELOG("RedisScriptManager::StringGet fail:key=%s, ret=%d", key.c_str(), ret);
	}
	return (ret == 0);
}
