#include "debug.h"

#include "redis_manager.h"
#include "redis_script_manager.h"

const char *HOST = "10.236.100.244";
//const char *HOST = "127.0.0.1";
const uint32_t PORT = 6379;
const char *PASSWORD = "foobared";
//const char *PASSWORD = "huiwang";

int main()
{
	NLOG("============TestRedisScript begin============");
	sRedisManager.NewInstance();

	if (sRedisManager.Init(HOST, PORT, PASSWORD))
	{
		sRedisScriptManager.NewInstance();

		int ret = 0;
		srand(time(nullptr));
		std::string key = "user";
		std::string value = "xiaohuihui" + std::to_string(rand() % 1000);
		std::string value_get;
		do
		{
			if (!sRedisScriptManager.RegisterScript())
			{
				ret = -1;
				break;
			}

			if (!sRedisScriptManager.StringSet(key, value))
			{
				ret = -2;
				break;
			}

			if (!sRedisScriptManager.StringGet(key, value_get))
			{
				ret = -3;
				break;
			}
		} while (false);
		NLOG("redis_script:ret=%d, key=%s, value=%s, value_get=%s", ret, key.c_str(), value.c_str(), value_get.c_str());

		sRedisScriptManager.DeleteInstance();
	}

	sRedisManager.DeleteInstance();
	NLOG("============TestRedisScript end============");
	return 0;
}
