#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <cassert>

// 对象的类型
enum XID_TYPE
{
	XT_NONE					= 0,	// 无类型

	XT_WIND					= 100,	// 测试类型Wind
	XT_SNOW_1				= 101,	// 测试类型Snow
	XT_SNOW_2				= 102,	// 测试类型Snow

	XT_COUNT				= XT_SNOW_2,

	XT_MAX					= 0x7f,
};

static bool IsValidXIDType(int type)
{
	return (type >= XT_NONE && type <= XT_COUNT);
}

#define XID_ID_MAX 0x00ffffffffffffff
static bool IsValidXIDId(int id)
{
	return (id >= 0 && id <=XID_ID_MAX);
}

#define XID_TYPE_LEFT_MOVE_BIT 56
#define XID_TYPE_MASK 0xff00000000000000
#define XID_ID_MASK 0x00ffffffffffffff

// 对象的xid
struct XID
{
	uint64_t id;

	XID() : id(0) {}
	XID(int _type, uint64_t _id)
	{
		if (!IsValidXIDType(_type) || !IsValidXIDId(_id))
		{
			assert(false);
		}
		id = ((_id & XID_ID_MASK) | ((int64_t)_type << XID_TYPE_LEFT_MOVE_BIT));
	}
	XID(const XID &rhs) { id = rhs.id; }

	XID_TYPE GetType() const { return (XID_TYPE)(id & XID_TYPE_MASK); }

	bool operator==(const XID& rhs) const { return id == rhs.id; }
	bool operator<(const XID & rhs) const { return id < rhs.id; }
	bool operator<=(const XID & rhs) const { return id <= rhs.id; }
	bool operator!=(const XID& rhs) const { return id != rhs.id; }

	void operator=(const XID &rhs) { id = rhs.id; }
};

std::string GetTimeStr(time_t t, bool is_simple = false)
{
	struct tm time_tm;
	localtime_r(&t, &time_tm);
	char buf[64] = {0};
	if (is_simple)
	{
		strftime(buf, sizeof(buf), "%d %H:%M:%S", &time_tm);
	}
	else
	{
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &time_tm);
	}
	return buf;
}

#endif
