#ifndef TS_QUEUE_H
#define TS_QUEUE_H

#include <queue>
#include <mutex>

// 线程安全队列
template<typename T>
class TSQueue
{
public:
	TSQueue() {}
	virtual ~TSQueue() {}

public:
	bool Empty() const
	{
		return m_ts_queue.empty();
	}

	size_t Size() const
	{
		return m_ts_queue.size();
	}

	bool Front(T &t)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		if (!m_ts_queue.empty())
		{
			t = m_ts_queue.front();
			return true;
		}
		return false;
	}

	bool Back(T &t)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		if (!m_ts_queue.empty())
		{
			t = m_ts_queue.back();
			return true;
		}
		return false;
	}

	bool Push(const T &t)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		m_ts_queue.push(t);
	}

	bool Pop(T &t)
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		if (!m_ts_queue.empty())
		{
			t = m_ts_queue.front();
			m_ts_queue.pop();
			return true;
		}
		return false;
	}

protected:
	std::queue<T> m_ts_queue;
	std::mutex m_mutex;
};

#endif
