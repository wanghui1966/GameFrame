#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "../common.h"
#include <mutex>

class Object
{
public:
	Object() {}
	virtual ~Object() {}

public:
	void Lock() { m_mutex.lock(); }
	void Unlock() { m_mutex.unlock(); }
	bool TryLock() { m_mutex.try_lock(); }

	void SetXID(const XID &xid) { m_xid = xid; }
	const XID& GetXID() const { return m_xid; }

	void SetActive() { m_active = true; }
	bool IsActive() { return m_active; }

public:
	void Clear()
	{
		m_active = false;
	}

	virtual void Heartbeat() {}

public:
	Object					*m_prev = nullptr;		// 前一个对象指针
	Object					*m_next = nullptr;		// 后一个对象指针

protected:
	XID						m_xid;					// id
	bool					m_active = false;		// 是否已激活

	std::mutex				m_mutex;
};

#endif
