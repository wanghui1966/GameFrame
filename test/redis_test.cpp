
#include "debug.h"
#include "common.h"

#include "hiredis.h"
void TestRedis()
{
	NLOG("============TestRedis begin============");
	redisContext *conn = redisConnect("127.0.0.1", 6379);
	if (!conn || conn->err)
	{
		ELOG("connect err");
	}

	redisReply *reply = (redisReply*)redisCommand(conn, "AUTH huiwang");
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
		if (!redis.Init("127.0.0.1", 6379, "huiwang"))
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
		if (!sRedisManager.Init("127.0.0.1", 6379, "huiwang"))
		{
			break;
		}

		for (int i = 0; i < 20; ++i)
		{
			std::thread th([=]{
				Redis *redis = sRedisManager.Get();
				if (redis)
				{
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

					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
				else
				{
					ELOG("PING:%d fail for redis", i);
				}
			});
			th.detach();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
		if (!sRedisManager.Init("127.0.0.1", 6379, "huiwang"))
		{
			break;
		}

		Redis *redis = sRedisManager.Get();
		if (redis)
		{
			SCOPE_GUARD([&]{ sRedisManager.Put(redis); });

			std::string cmd = "set user xiaohuihui_";
			cmd.append(std::to_string(time(nullptr)));
			bool is_ok = SetRedis(redis, cmd);
			NLOG("%s:is_ok=%d", cmd.c_str(), is_ok);

			std::string user;
			is_ok = GetRedisString(redis, "get user", user);
			NLOG("get user:is_ok=%d, user=%s", is_ok, user.c_str());
		}
		else
		{
			ELOG("fail for redis");
		}
	} while(false);

	sRedisManager.DeleteInstance();
	NLOG("============TestRedisManager end============\n");
}

int main()
{
	TestRedis();
	TestHiRedis();
	// TestRedisManagerThread();
	TestRedisManager();
	return 0;
}
