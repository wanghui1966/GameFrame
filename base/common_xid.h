#pragma once

#ifndef COMMON_XID_H
#define COMMON_XID_H

#include <cstdint>
#include <cassert>

// 对象的类型
enum XID_TYPE
{
	XT_NONE					= 0,				// 无类型

	XT_WIND					= 2000,				// 测试类型Wind
	XT_SNOW_1				= 2001,				// 测试类型Snow
	XT_SNOW_2				= 2002,				// 测试类型Snow

	XT_SHM_WIND				= 2010,				// 共享内存测试类型Wind

	XT_COUNT				= XT_SNOW_2,

	XT_MAX					= 0x7ff,			// 2047
};

static bool IsValidXIDType(int type)
{
	return (type >= XT_NONE && type <= XT_COUNT);
}

#define XID_ID_MAX 0x000fffffffffffff
static bool IsValidXIDId(int id)
{
	return (id >= 0 && id <= XID_ID_MAX);
}

#define XID_TYPE_LEFT_MOVE_BIT 52
#define XID_TYPE_MASK 0xfff0000000000000
#define XID_ID_MASK 0x000fffffffffffff

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

#endif
