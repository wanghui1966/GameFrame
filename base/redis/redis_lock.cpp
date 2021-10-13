#include "redis_lock.h"

#include "../snow_flake.h"
#include "redis_manager.h"

RedisLock::RedisLock()
{
	Init();
}

RedisLock::RedisLock(int continue_count, int continue_time) : m_expire_continue_count(continue_count), m_expire_continue_time(continue_time)
{
	Init();
}

bool RedisLock::Lock()
{
	//int ret = sRedisManager.ExecuteCommandReturnError(error, "set user huiwang");
}

bool RedisLock::Unlock()
{
}

void RedisLock::Init()
{
	SnowFlake sf;
	sf.Init(LOCAL_DATA_CENTER_ID, LOCAL_MACHINE_ID);
	m_reids_lock_key = sf.GetNextId();
}
