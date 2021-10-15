
#include "debug.h"
#include "common.h"
#include "common_string.h"

const char *HOST = "10.236.100.244";
//const char *HOST = "127.0.0.1";
const uint32_t PORT = 6379;
const char *PASSWORD = "foobared";
//const char *PASSWORD = "huiwang";

#include "hiredis.h"
void TestRedis()
{
	NLOG("============TestRedis begin============");
	redisContext *conn = redisConnect(HOST, PORT);
	if (!conn || conn->err)
	{
		ELOG("connect err");
	}

	redisReply *reply = (redisReply*)redisCommand(conn, "AUTH %s", PASSWORD);
	NLOG("AUTH:%s", reply->str);
	reply = (redisReply*)redisCommand(conn, "PING");
	NLOG("PING:%s", reply->str);
	freeReplyObject(reply);

	redisFree(conn);
	NLOG("============TestRedis end============\n");
}

#include "redis_manager.h"
void TestHiRedis()
{
	NLOG("============TestHiRedis begin============");
	do
	{
		Redis redis;
		if (!redis.Init(HOST, PORT, PASSWORD))
		{
			break;
		}

		if (!redis.Connect())
		{
			break;
		}

		RedisReply redis_reply;
		if (!redis.ExecuteCommand(redis_reply, "PING"))
		{
			break;
		}
		if (!redis_reply.IsStatus())
		{
			break;
		}
		std::string status;
		if (!redis_reply.GetStatus(status))
		{
			break;
		}
		NLOG("PING:%s", status.c_str());

		if (!redis.ExecuteCommand(redis_reply, "get user"))
		{
			break;
		}
		if (!redis_reply.IsString())
		{
			break;
		}
		std::string str;
		if (!redis_reply.GetString(str))
		{
			break;
		}
		NLOG("user:%s", str.c_str());

		if (!redis.ExecuteCommand(redis_reply, "set user xiaohuihui"))
		{
			break;
		}

		if (!redis.ExecuteCommand(redis_reply, "get user"))
		{
			break;
		}
		if (!redis_reply.IsString())
		{
			break;
		}
		if (!redis_reply.GetString(str))
		{
			break;
		}
		NLOG("user:%s", str.c_str());
	}
	while (false);
	NLOG("============TestHiRedis end============\n");
}

#include <thread>
#include <chrono>
void TestRedisManagerThread()
{
	NLOG("============TestRedisManagerThread begin============");
	sRedisManager.NewInstance();

	do
	{
		if (!sRedisManager.Init(HOST, PORT, PASSWORD))
		{
			break;
		}

		for (int i = 0; i < 20; ++i)
		{
			std::thread th([=]{
				Redis *redis = sRedisManager.Get();
				if (redis)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));

					SCOPE_GUARD([&]{ sRedisManager.Put(redis); });

					RedisReply redis_reply;
					if (!redis->ExecuteCommand(redis_reply, "PING"))
					{
						ELOG("PING:%d fail for ExecuteCommand", i);
						return;
					}
					if (!redis_reply.IsStatus())
					{
						ELOG("PING:%d fail for IsStatus", i);
						return;
					}
					std::string status;
					if (!redis_reply.GetStatus(status))
					{
						ELOG("PING:%d fail for GetStatus", i);
						return;
					}
					NLOG("PING:%d success:%s", i, status.c_str());

				}
				else
				{
					ELOG("PING:%d fail for redis", i);
				}
			});
			th.join();
		}
	} while(false);

	sRedisManager.DeleteInstance();
	NLOG("============TestRedisManagerThread end============\n");
}

bool GetRedisString(Redis *redis, const std::string &cmd, std::string &str)
{
	if (!redis || cmd.empty())
	{
		return false;
	}

	RedisReply redis_reply;
	if (!redis->ExecuteCommand(redis_reply, cmd.c_str()) || !redis_reply.IsString() || !redis_reply.GetString(str))
	{
		return false;
	}

	return true;
}
bool SetRedis(Redis *redis, const std::string &cmd)
{
	if (!redis || cmd.empty())
	{
		return false;
	}

	RedisReply redis_reply;
	if (!redis->ExecuteCommand(redis_reply, cmd.c_str()) || redis_reply.IsError())
	{
		return false;
	}

	return true;
}
void TestRedisManager()
{
	NLOG("============TestRedisManager begin============");
	sRedisManager.NewInstance();

	do
	{
		if (!sRedisManager.Init(HOST, PORT, PASSWORD))
		{
			break;
		}

		// set，设置/更新key的值，带ex/px为设置/更新过期时间(秒/毫秒)，带nx为key不存在才设置，带xx为key存在才设置
		// mset
		// setex，设置/更新key的值和过期时间
		//int ExecuteCommandReturnError(std::string &error, const char *format, ...);

		// del，返回删除key数量
		// expire，设置/更新key的过期时间(秒)，成功返回1，失败返回0
		// ttl，返回key的过期时间(秒)，永久返回-1，key不存在返回-2
		// pexpire，设置/更新key的过期时间(毫秒)，成功返回1，失败返回0
		// pttl，返回key的过期时间(毫秒)，永久返回-1，key不存在返回-2
		// persist，移除key的过期时间，成功返回1，失败返回0
		// setnx，若key不存在设置key的值返回1，否则返回0
		//int ExecuteCommandReturnInteger(long long &result, const char *format, ...);

		// script load
		// get，key不存在返回失败
		// getset，成功返回旧值，key不存在时会返回失败但是key的值设置成功
		//int ExecuteCommandReturnString(std::string &result, const char *format, ...);

		// mget，返回所有key的值
		//int ExecuteCommandReturnStrings(std::vector<std::string> &results, const char *format, ...);
		{
			RedisReply redis_reply;
			bool ret = false;

			//std::vector<std::string> results;
			//std::string results_str;
			//ret = sRedisManager.ExecuteCommand(redis_reply, "mget user user2 user1");
			//if (ret && redis_reply.GetArrayStrings(results))
			//{
			//	StringHelper::GetVectorStr<std::string>(results, results_str);
			//}
			//NLOG("ret=%d, results_str=%s", ret, results_str.c_str());

			ret = sRedisManager.ExecuteCommand(redis_reply, "set user 1 nx");
			NLOG("ret=%d, type=%d", ret, redis_reply.GetType());
		}

		//Redis *redis = sRedisManager.Get();
		//if (redis)
		//{
		//	SCOPE_GUARD([&]{ sRedisManager.Put(redis); });

		//	std::string cmd = "set user xiaohuihui_";
		//	cmd.append(std::to_string(time(nullptr)));
		//	bool is_ok = SetRedis(redis, cmd);
		//	NLOG("%s:is_ok=%d", cmd.c_str(), is_ok);

		//	std::string user;
		//	is_ok = GetRedisString(redis, "get user", user);
		//	NLOG("get user:is_ok=%d, user=%s", is_ok, user.c_str());
		//}
		//else
		//{
		//	ELOG("fail for redis");
		//}
	} while(false);

	sRedisManager.DeleteInstance();
	NLOG("============TestRedisManager end============\n");
}

int main()
{
	//TestRedis();
	//TestHiRedis();
	TestRedisManagerThread();
	//TestRedisManager();
	return 0;
}
