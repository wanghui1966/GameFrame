#pragma once

#ifndef REDIS_LOCK_H
#define REDIS_LOCK_H

#include "debug.h"

const int REDIS_LOCK_EXPIRE_TIME = 10;			// 锁的超时时间

// 应该来源于配置
const int LOCAL_DATA_CENTER_ID = 6666;
const int LOCAL_MACHINE_ID = 8888;

// 使用此类前，须先初始化RedisManager！！！
class RedisLock
{
public:
	RedisLock()
	RedisLock(int continue_count, int continue_time);

public:
	bool Lock();
	bool Unlock();

protected:
	void Init();

protected:
	int64_t m_reids_lock_key						= 0;		// 锁的名字，用来区分，避免被其他进程给删除了
	int m_expire_time								= REDIS_LOCK_EXPIRE_TIME;		// 锁的超时时间
	int m_expire_continue_count						= 0;		// 锁超时后，继续持有锁的次数
	int m_expire_continue_time						= 0;		// 锁超时后，继续持有锁的持续时间
};

#endif
