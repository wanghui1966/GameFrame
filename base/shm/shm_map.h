#pragma once

#ifndef SHM_MAP_H
#define SHM_MAP_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <mutex>

// ！！！！！！由于mutex不支持拷贝构造和赋值操作，所以不支持动态增长操作，需要自己实现锁函数Lock(int *l)

// kv容器
// 使用unordered_map实现，整个容器带一把锁，容器内每个value也带一把锁
// 容器内的锁必须自己实现，必须支持Lock(int *i)操作
template<typename KEY, typename T>
class ShmMap
{
private:
	struct Node
	{
		std::mutex						m_mutex;		// 结点锁
		T								m_data;			// 结点数据

		Node() {}
		Node(const T &data) : m_data(data) {}
	};

public:
	ShmMap() {}
	virtual ~ShmMap() {}

public:
	// 是否拥有指定key数据
	bool HasData(KEY key)
	{
		std::lock_guard<std::mutex> lck_data(m_mutex);
		return (m_data_map.find(key) != m_data_map.end());
	}

	// 插入数据 
	bool InsertData(KEY key, const T &data)
	{
		std::lock_guard<std::mutex> lck_data(m_mutex);
		auto it = m_data_map.find(key);
		if (it == m_data_map.end())
		{
			m_data_map.insert(std::make_pair(key, Node()));
			return true;
		}
		return false;
	}

	// 获取指定key的数据，是否需要插入
	bool GetData(KEY key, T &data, bool insert = false)
	{
		std::lock_guard<std::mutex> lck_data(m_mutex);
		auto it = m_data_map.find(key);
		if (it == m_data_map.end())
		{
			if (insert)
			{
				it = m_data_map.insert(std::make_pair(key, Node()));
			}
			else
			{
				return false;
			}
		}
		std::lock_guard<std::mutex> lck_value(it->second.m_mutex);
		data = it->second.m_data;
		return true;
	}

	// 删除指定key的数据
	bool DelData(KEY key)
	{
		std::lock_guard<std::mutex> lck_data(m_mutex);
		auto it = m_data_map.find(key);
		if (it == m_data_map.end())
		{
			return false;
		}
		m_data_map.erase(it);
		return true;
	}

	// 指定key的数据，执行一个函数
	template<typename T1>
	T1 lock_and_run(KEY key, std::function<T1(T* data)> func, bool insert = false)
	{
		T1 ret;
		m_mutex.lock();
		auto it = m_data_map.find(key);
		if (it == m_data_map.end())
		{
			if (insert)
			{
				it = m_data_map.insert(std::make_pair(key, Node()));
			}
			else
			{
				m_mutex.unlock();
				try
				{
					ret = func(nullptr);
				}
				catch (...) { }
				return ret;
			}
		}
		std::lock_guard<std::mutex> lck_data(it->second.m_mutex);
		m_mutex.unlock();// 将m_data_map的锁释放，只锁单个结点

		try
		{
			ret = func(&it->second.m_data);
		}
		catch (...) { }
		return ret;
	}

	// 指定多组key的数据，执行一个函数
	// 由于需要对keys排序，所以KEY如果是自定义结构一定要实现<=操作符
	template<typename T1>
	T1 lock_and_run(std::vector<KEY>& keys, std::function<T1(std::vector<T*>& datas)> func, bool insert = false)
	{
		T1 ret;
		std::vector<T*> datas;
		if (keys.empty())
		{
			try
			{
				ret = func(datas);
			}
			catch (...) { }
			return ret;
		}
		// 必须排序，保证下面的上锁不会出现死锁情况
		std::sort(keys.begin, keys.end());

		m_mutex.lock();
		for (size_t i = 0; i < keys.size(); ++i)
		{
			auto it = m_data_map.find(keys[i]);
			if (it == m_data_map.end())
			{
				if (insert)
				{
					it = m_data_map.insert(std::make_pair(keys[i], Node()));
					it->second.lock();
					datas.push_back(&it->second.m_data);
				}
				else
				{
					datas.push_back(nullptr);
				}
			}
			else
			{
				it->second.lock();
				datas.push_back(&it->second.m_data);
			}
		}
		m_mutex.unlock();// 将m_data_map的锁释放，只锁单个结点

		try
		{
			ret = func(datas);
		}
		catch (...) { }

		for (size_t i = 0; i < keys.size(); ++i)
		{
			if (!datas[i])
			{
				continue;
			}
			auto it = m_data_map.find(keys[i]);
			if (it != m_data_map.end())
			{
				it->second.m_mutex.unlock();
			}
		}
		return ret;
	}

protected:
	std::mutex								m_mutex;		// 全局锁
	std::unordered_map<KEY, struct Node>	m_data_map;		// 全局数据
};

#endif
