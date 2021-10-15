#include "debug.h"

#include "redis_manager.h"
#include "redis_lock.h"

#include <chrono>
#include <thread>

const char *HOST = "10.236.100.244";
//const char *HOST = "127.0.0.1";
const uint32_t PORT = 6379;
const char *PASSWORD = "foobared";
//const char *PASSWORD = "huiwang";

RedisLock rl;
void thread_func()
{
	int count = 3;
	while (count)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
		bool lock_ret = rl.Lock();
		NLOG("thread_id=%d, key=%s, lock_ret=%d", std::this_thread::get_id(), rl.GetKey().c_str(), lock_ret);
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
		bool unlock_ret = rl.Unlock();
		NLOG("thread_id=%d, key=%s, unlock_ret=%d", std::this_thread::get_id(), rl.GetKey().c_str(), unlock_ret);
		--count;
	};
}

int main()
{
	NLOG("============TestRedisLock begin============");
	sRedisManager.NewInstance();

	if (sRedisManager.Init(HOST, PORT, PASSWORD))
	{
		std::thread threads[20];
		for (std::thread &th : threads)
		{
			th = std::thread(thread_func);
		}
		for (std::thread &th : threads)
		{
			th.join();
		}
	}

	sRedisManager.DeleteInstance();
	NLOG("============TestRedisLock end============");
	return 0;
}
