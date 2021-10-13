#pragma once

#ifndef SNOW_FLAKE_H
#define SNOW_FLAKE_H

#include <chrono>
//#include <sys/time.h>

#include "debug.h"

// 0 000 0000 0000 0000 0 000 0000 000 0 0000 0000 0000 00 00 0000 0000 0000 0000 0000
// | |---------------------------------------------------| |----------| |------------|
//                           时间戳                          机器标识       序列号

const int64_t SEQUENCE_BIT					= 12;	// 序列号占用位数
const int64_t MACHINE_BIT					= 5;	// 机器标识占用位数
const int64_t DATA_CENTER_BIT				= 5;	// 数据中心占用位数
const int64_t TIMESTAMP_BIT					= 41;	// 时间戳占用位数

// 各部分最大值
const int64_t MAX_SEQUENCE_NUM = -1L ^ (-1L << SEQUENCE_BIT);
const int64_t MAX_MACHINE_NUM = -1L ^ (-1L << MACHINE_BIT);
const int64_t MAX_DATA_CENTER_NUM = -1L ^ (-1L << DATA_CENTER_BIT);

// 左移位数
const int64_t MACHINE_LEFT = SEQUENCE_BIT;
const int64_t DATA_CENTER_LEFT = SEQUENCE_BIT + MACHINE_BIT;
const int64_t TIMESTAMP_LEFT = SEQUENCE_BIT + MACHINE_BIT + DATA_CENTER_BIT;

// 开始时间戳，1970-01-01
const int64_t BEGIN_TIMESTAMP = 0;

class SnowFlake
{
public:
	SnowFlake() {}
	~SnowFlake() {}

public:
	bool Init(int64_t data_center_id, int64_t machine_id)
	{
		if (data_center_id < 0 || data_center_id > MAX_DATA_CENTER_NUM)
		{
			return false;
		}
		if (machine_id < 0 || machine_id > MAX_MACHINE_NUM)
		{
			return false;
		}

		m_data_center_id = data_center_id;
		m_machine_id = machine_id;
		NLOG("SnowFlake::Init success:m_data_center_id=%ld, m_machine_id=%ld", m_data_center_id, m_machine_id);
		return true;
	}

	int64_t GetNextId()
	{
		int64_t now_timestamp = GetNowTimestamp();
		if (now_timestamp < m_last_timestamp)
		{
			return false;
		}

		if (now_timestamp == m_last_timestamp)
		{
			// 相同毫秒内，序列号自增
			m_sequence_id = (m_sequence_id + 1) & MAX_SEQUENCE_NUM;
			if (m_sequence_id == 0)
			{
				// 同一毫秒的序列数已经达到最大
				while (now_timestamp <= m_last_timestamp)
				{
					now_timestamp = GetNowTimestamp();
				}
			}
		}
		else
		{
			// 不同毫秒内，序列号置为0
			m_sequence_id = 0;
		}
		m_last_timestamp = now_timestamp;

		return ((now_timestamp - BEGIN_TIMESTAMP) << TIMESTAMP_LEFT) | (m_data_center_id << DATA_CENTER_LEFT) | (m_machine_id << MACHINE_LEFT) | m_sequence_id;
	}

protected:
	// 毫米级别的时间戳
	int64_t GetNowTimestamp()
	{
		//timeval tv;
		//gettimeofday(&tv, 0);
		//return (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

protected:
	int64_t m_last_timestamp					= 0;			// 上次获取id的时间戳(毫秒)
	int64_t m_data_center_id					= 0;			// 数据中心id
	int64_t m_machine_id						= 0;			// 机器标识id
	int64_t m_sequence_id						= 0;			// 同一时间戳(毫秒)序号
};

#endif
