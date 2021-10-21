#pragma once

#ifndef REDIS_LOCK_H
#define REDIS_LOCK_H

#include "debug.h"

#include <string>

// 应该来源于配置
const int LOCAL_DATA_CENTER_ID = 6666;
const int LOCAL_MACHINE_ID = 8888;

// 上锁尝试次数
const int REDIS_LOCK_RETRY_COUNT = 10;
// 上锁尝试间隔(毫秒)
const int REDIS_LOCK_RETRY_INTERVAL = 1000;

// 使用此类的Lock系列函数前，须先初始化RedisManager！！！
class RedisLock
{
public:
	RedisLock();
	RedisLock(const std::string &redis_lock_key);
	virtual ~RedisLock();

public:
	bool HasLock();
	bool TryLock();
	bool Lock();
	bool Unlock();

public:
	std::string GetKey();

protected:
	void Init();

protected:
	std::string m_reids_lock_key;		// 锁的名字，用来区分，避免被其他进程给删除了
};

#endif
