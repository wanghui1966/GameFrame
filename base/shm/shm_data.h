#pragma once

#ifndef SHM_DATA_H
#define SHM_DATA_H

#include <mutex>

#include "shm_map.h"

class ShmData
{
public:
	ShmData() {}
	~ShmData() {}

public:
	// 1. 一般数据
	std::mutex							m_common_mutex;
	int									m_common_data = 0;

	// 2. 可以动态增长和删除的结构
	// ！！！！！！由于mutex不支持拷贝构造和赋值操作，所以不支持动态增长操作，需要自己实现锁函数Lock(int *l)
	//ShmMap<int64_t, int>				m_shm_map_test;
};

#endif
