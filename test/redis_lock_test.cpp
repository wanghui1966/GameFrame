#include "debug.h"

#include "redis_manager.h"
#include "redis_lock.h"

#include <chrono>
#include <thread>
#include <iostream>

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
		std::cout << "thread_id=" << std::this_thread::get_id() << ", key=" << rl.GetKey() << ", lock_ret=" << lock_ret << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
		if (!lock_ret)
		{
			continue;
		}
		bool unlock_ret = rl.Unlock();
		std::cout << "thread_id=" << std::this_thread::get_id() << ", key=" << rl.GetKey() << ", unlock_ret=" << unlock_ret << std::endl;
		--count;
	};
}

int main()
{
	NLOG("============TestRedisLock begin============");
	sRedisManager.NewInstance();

	if (sRedisManager.Init(HOST, PORT, PASSWORD))
	{
		std::thread threads[10];
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
