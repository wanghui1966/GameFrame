#pragma once

#ifndef OBJECT_POOL_MANAGER_H
#define OBJECT_POOL_MANAGER_H

#include <cstdint>
#include <mutex>
#include <vector>
#include <set>
#include <map>

#include <cassert>
#include "../debug.h"

// 多线程对象池的基本实现
// OBJECT为对象类型，必须继承于object.h
// INDEX_MIN为索引最小值
// INDEX_MAX为索引最大值
// INC_POOL_SIZE为对象池满了之后每次扩充的对象池大小
template<typename OBJECT, size_t INDEX_MIN = 0, size_t INDEX_MAX = 0, size_t INC_POOL_SIZE = 0>
class ObjectPoolManagerBasic
{
	class ObjectPool
	{
		public:
			ObjectPool() : m_pool(nullptr), m_pool_size(0), m_next_pool(nullptr) {}
			~ObjectPool()
			{
				if (m_pool)
				{
					for (size_t i = 0; i < m_pool_size; ++i)
					{
						OBJECT* obj = &m_pool[i];
						obj->~OBJECT();
					}
					free(m_pool);
					m_pool = nullptr;
				}
				m_next_pool = nullptr;
			}

		public:
			// 初始化函数
			bool Init(size_t size)
			{
				// 分配内存
				m_pool = reinterpret_cast<OBJECT*>(calloc(size, sizeof(OBJECT)));
				if (!m_pool)
				{
					return false;
				}

				// 调用placement new，初始化对象
				for (size_t i = 0; i < size; ++i)
				{
					OBJECT* obj = &m_pool[i];
					new (obj) OBJECT;
					if (i != 0)
					{
						obj->m_prev = m_pool + (i - 1);
					}
					if (i != size - 1)
					{
						obj->m_next = m_pool + (i + 1);
					}
				}
				m_pool[0].m_prev = nullptr;
				m_pool[size - 1].m_next = nullptr;

				m_pool_size = size;
				return true;
			}

			// 根据对象池索引得到对应索引首地址
			OBJECT* GetByIndex(size_t index) const
			{
				if (index < m_pool_size)
				{
					return m_pool + index;
				}
				if (m_next_pool)
				{
					return m_next_pool->GetByIndex(index - m_pool_size);
				}
				return nullptr;
			}

			// 根据对象池首地址和偏移得到对象池索引
			size_t GetIndex(OBJECT* obj, size_t offset) const
			{
				if (obj >= m_pool && obj < m_pool + m_pool_size)
				{
					return obj - m_pool + offset;
				}
				if (m_next_pool)
				{
					return m_next_pool->GetIndex(obj, offset + m_pool_size);
				}
				return INT64_MAX;
			}

			// 是否对象池内对象
			bool IsPoolObject(OBJECT* obj) const
			{
				if (obj >= m_pool && obj < m_pool + m_pool_size)
				{
					return true;
				}
				return (m_next_pool && m_next_pool->IsPoolObject(obj));
			}

		public:
			OBJECT			*m_pool;
			size_t			m_pool_size;
			ObjectPool		*m_next_pool;
	};

public:
	ObjectPoolManagerBasic(const char *name) : m_obj_name(name), m_head(nullptr), m_tail(nullptr), m_count(0), m_pool_size(0), m_pool(nullptr) {}
	virtual ~ObjectPoolManagerBasic()
	{
		std::vector<ObjectPool*> to_del;
		ObjectPool *p = m_pool;
		while (p)
		{
			to_del.push_back(p);
			p = p->m_next_pool;
		}
		for (auto it = to_del.begin(); it != to_del.end(); ++it)
		{
			delete *it;
		}
		m_pool = nullptr;
	}

public:
	// 初始化
	bool Init(size_t size)
	{
		if (m_pool)
		{
			ELOG("ObjectPoolManagerBasic::Init");
			assert(false);
			return false;
		}
		ObjectPool *p = new ObjectPool();
		if (!p->Init(size))
		{
			return false;
		}
		AppendPool(p);
		return true;
	}

	// 根据对象池索引得到对应索引首地址
	OBJECT* GetByIndex(size_t index) const
	{
		if (index < INDEX_MIN + m_pool_size)
		{
			return m_pool->GetByIndex(index - INDEX_MIN);
		}
		return nullptr;
	}

	// 根据对象池首地址和偏移得到对象池索引
	size_t GetIndex(OBJECT* obj) const 
	{ 
		return INDEX_MIN + m_pool->GetIndex(obj, 0);
	}

	bool IsPoolObject(OBJECT* obj) const 
	{ 
		return m_pool->IsPoolObject(obj);
	}

	// 分配成功时，OBJECT对象已经上锁，注意在外面放开锁！！！
	virtual OBJECT* Alloc()
	{
		OBJECT *obj = nullptr;
		{
			std::lock_guard<std::mutex> lck(m_mutex);

			// 表示现有的池子已经分配干净了
			if (!m_head)
			{
				// 对象池不允许增长
				if (INC_POOL_SIZE == 0)
				{
					return nullptr;
				}

				// 对象池配置了索引最大值，且已经超过了
				if (INDEX_MAX > 0 && m_pool_size + INC_POOL_SIZE > INDEX_MAX - INDEX_MIN)
				{
					//池子的大小超过了索引的范围
					return nullptr;
				}

				ObjectPool *p = new ObjectPool();
				if (!p->Init(INC_POOL_SIZE))
				{
					return nullptr;
				}
				AppendPool(p);
			}

			obj = m_head;
			m_head = (OBJECT*)m_head->m_next;
			if (m_head)
			{
				m_head->m_prev = nullptr;
			}
			else
			{
				// 现有的池子已经耗尽
				m_tail = nullptr;
			}
			++m_count;
		}

		if (!obj || obj->IsActive())
		{
			return nullptr;
		}
		obj->Lock();
		obj->SetActive();
		return obj;
	}
	
	// OBJECT对象已经上锁，释放完毕后在外面放开锁！！！
	void Free(OBJECT *obj)
	{
		if (!obj->IsActive())
		{
			ELOG("ObjectPoolManagerBasic::Free");
			assert(false);
			return;
		}
		obj->Clear();
	
		{
			std::lock_guard<std::mutex> lck(m_mutex);
			if (!m_tail)
			{
				// 现有的池子已经耗尽
				m_head = obj;
				m_tail = obj;
				obj->m_prev = nullptr;
				obj->m_next = nullptr;
			}
			else
			{
				m_tail->m_next = obj;
				obj->m_prev = m_tail;
				obj->m_next = nullptr;
				m_tail = obj;
			}
			--m_count;
		}
	}

	size_t GetCount() const { return m_count; }
	size_t GetPoolSize() const { return m_pool_size; }
	size_t GetBeginIndex() const { return INDEX_MIN; }

	void ForEachObject(std::function<void(OBJECT *)> func)
	{
		ObjectPool *p = m_pool;
		size_t count = 0;
		while (p)
		{
			OBJECT *obj = p->m_pool;
			for (; obj < p->m_pool + p->m_pool_size; ++obj)
			{
				func(obj);
				++count;
			}
			p = p->m_next_pool;
		}
	}

protected:
	void AppendPool(ObjectPool *p)
	{
		if (!m_tail)
		{
			m_head = p->m_pool;
			m_tail = p->m_pool + p->m_pool_size - 1;
		}
		else
		{
			m_tail->m_next = p->m_pool;
			p->m_pool->m_prev = m_tail;
			m_tail = p->m_pool + p->m_pool_size - 1;
		}

		if (m_pool)
		{
			ObjectPool *op = m_pool;
			while (op->m_next_pool)
			{
				op = op->m_next_pool;
			}
			op->m_next_pool = p;
		}
		else
		{
			m_pool = p;
		}
		m_pool_size += p->m_pool_size;
	}

protected:
	std::mutex				m_mutex;

	const char				*m_obj_name;		// 对象的名字

	// 对象池为空的时候，m_head为第一个对象，m_tail为第二个对象
	// 分配对象的时候，拿出m_head，并将m_head往后挪
	// 销毁对象的时候，将对象放到m_tail的位置，m_tail指向这个对象
	// 当m_head==m_tail==nullptr的时候表示对象池已经耗尽
	OBJECT					*m_head;			// 对象池的当前头指针
	OBJECT					*m_tail;			// 对象池的当前尾指针

	size_t					m_count;			// 对象池的数量(总共几个ObjectPool)
	size_t					m_pool_size;		// 对象池的总大小
	ObjectPool				*m_pool;			// 对象池的头指针
};

// 一个带心跳的对象池实现
// HEARTBEAT_TICK为执行完所有对象的心跳所需要的总心跳次数(比如如果想要在100次心跳内执行完对象池内所有对象的心跳，则配置为100)
template<typename OBJECT, size_t HEARTBEAT_TICK, size_t INDEX_MIN = 0, size_t INDEX_MAX = 0, size_t INC_POOL_SIZE = 0>
class ObjectPoolManager : public ObjectPoolManagerBasic<OBJECT, INDEX_MIN, INDEX_MAX, INC_POOL_SIZE>
{
public:
	ObjectPoolManager(const char * name) : ObjectPoolManagerBasic<OBJECT, INDEX_MIN, INDEX_MAX, INC_POOL_SIZE>(name), m_cur_obj(nullptr), m_cur_heartbeat_obj_count(0) {}
	virtual ~ObjectPoolManager() {}

public:
	void Insert(OBJECT *obj)
	{
		std::lock_guard<std::mutex> lck(m_mutex_change);
		if (m_change[obj] > 0)
		{
			ELOG("ObjectPoolManager::Insert");
			assert(false);
			return;
		}
		++m_change[obj];
	}

	void Remove(OBJECT *obj)
	{
		std::lock_guard<std::mutex> lck(m_mutex_change);
		if (m_change[obj] < 0)
		{
			ELOG("ObjectPoolManager::Remove");
			assert(false);
			return;
		}
		--m_change[obj];
	}
	
	void Heartbeat()
	{
		std::vector<OBJECT*> vv;
		{
			std::lock_guard<std::mutex> lck(m_mutex_heartbeat);
			CollectHeartbeatObject(vv);
		}
		if (vv.empty())
		{
			return;
		}

		for (auto it = vv.begin(); it != vv.end(); ++it)
		{
			if (*it)
			{
				(*it)->Heartbeat();
			}
		}
	}

	void DoDelete(OBJECT *obj)
	{
		if (m_cur_obj == obj)
		{
			m_cur_it = m_obj_set.find(m_cur_obj);
			++m_cur_it;
			if(m_cur_it == m_obj_set.end())
			{
				m_cur_obj = nullptr;
			}
			else
			{
				m_cur_obj = *m_cur_it;
			}
		}

		if (!m_obj_set.erase(obj))
		{
			ELOG("ObjectPoolManager::DoDelete");
			assert(false);
			return;
		}
	}

	void DoInsert(OBJECT *obj)
	{
		if (!m_obj_set.insert(obj).second)
		{
			ELOG("ObjectPoolManager::DoInsert");
			assert(false);
			return;
		}
	}

	void DoChange()
	{
		std::lock_guard<std::mutex> lck(m_mutex_change);
		for (auto it = m_change.begin(); it != m_change.end(); ++it)
		{
			if (it->second == 1)
			{
				DoInsert(it->first);
			}
			else if (it->second == -1)
			{
				DoDelete(it->first);
			}
			else if (it->second == 0)
			{
				// 表示调用改过同等数量的插入和删除，什么都不做
			}
			else
			{
				ELOG("ObjectPoolManager::DoChange");
				assert(false);
				return;
			}
		}
		m_change.clear();
	}

	// 1. 对象池上分配好对象后，调用Insert，在心跳中把加入的对象放入心跳中
	// 2. 对象池上的对象销毁后，调用Remove，在心跳中把销毁的对象从所有对象中移除
	// 3. 收集本次心跳 需要执行心跳的所有对象，保证所有对象的心跳平均分布，HEARTBEAT_TICK为1s内执行的心跳总数，假如为100
	// 如果当前对象数量为10，则每10次心跳会执行一遍所有对象的心跳一次
	// 如果当前对象数量为1000，则每次心跳会执行10个对象的心跳一次
	void CollectHeartbeatObject(std::vector<OBJECT*>& vv)
	{
		DoChange();

		size_t size = m_obj_set.size();
		if (size == 0)
		{
			// 一个对象都没有
			m_cur_heartbeat_obj_count = 0;
			m_cur_obj = nullptr;
			return ;
		}
		m_cur_heartbeat_obj_count += size;

		if (!m_cur_obj)
		{
			m_cur_it = m_obj_set.end();
		}
		else
		{
			m_cur_it = m_obj_set.find(m_cur_obj);
		}
		vv.reserve(size / HEARTBEAT_TICK + 5);

		for (; m_cur_heartbeat_obj_count >= HEARTBEAT_TICK; m_cur_heartbeat_obj_count -= HEARTBEAT_TICK)
		{
			if (m_cur_it == m_obj_set.end())
			{
				m_cur_it = m_obj_set.begin();
			}
			m_cur_obj = *m_cur_it;
			vv.push_back(m_cur_obj);
			++m_cur_it;
		}

		if (m_cur_it != m_obj_set.end())
		{
			m_cur_obj = *m_cur_it;
		}
		else
		{
			m_cur_obj = nullptr;
		}
	}

protected:
	std::mutex								m_mutex_change;
	std::mutex								m_mutex_heartbeat;

	std::set<OBJECT*>						m_obj_set;						// 所有拥有心跳的对象
	std::map<OBJECT*, int>					m_change;						// 本次心跳加入的对象和删除的对象
	typename std::set<OBJECT*>::iterator	m_cur_it;						// 当前心跳的对象所在位置
	OBJECT									*m_cur_obj;						// 当前心跳的对象
	size_t									m_cur_heartbeat_obj_count;		// 当前心跳对象总数量
};

#endif
