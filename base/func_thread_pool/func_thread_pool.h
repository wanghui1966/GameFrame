#pragma once

#ifndef FUNC_THREAD_POOL_H
#define FUNC_THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "../singleton.h"
#include "../debug.h"

class FuncThreadPool : public Singleton<FuncThreadPool>
{
public:
	FuncThreadPool() {}
	virtual ~FuncThreadPool() {}

	void Init(int thread_num);
	void Stop();

	template<class F, class... Args>
	auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
	// 可使用auto代替
	// auto enqueue(F &&f, Args &&... args) -> auto;

private:
	int m_thread_num = 0;							// 线程数量
	std::vector<std::thread> m_workers;				// 工作线程
	std::queue<std::function<void()>> m_tasks;		// 任务队列

	std::mutex m_mutex;
	std::condition_variable m_condition_variable;
	bool m_stop = false;
};

void FuncThreadPool::Init(int thread_num)
{
	m_thread_num = thread_num;
	for (int i = 0; i < m_thread_num; ++i)
	{
		m_workers.emplace_back(
				[this]
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->m_mutex);
							this->m_condition_variable.wait(lock, [this] { return (this->m_stop || !this->m_tasks.empty()); });
							if (this->m_stop && this->m_tasks.empty())
							{
								return;
							}
							task = std::move(this->m_tasks.front());
							this->m_tasks.pop();
						}
						task();
					}
				});
	}
	NLOG("FuncThreadPool::Init:thread_num=%d", m_thread_num);
}

void FuncThreadPool::Stop()
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_stop = true;
	}
	m_condition_variable.notify_all();
	for (std::thread &worker : m_workers)
	{
		worker.join();
	}
	NLOG("FuncThreadPool::Stop");
}

template<class F, class... Args>
auto FuncThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>...));

	std::future<return_type> ret = task->get_future();
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_stop)
		{
			throw std::runtime_error("enqueue on stopped FuncThreadPool");
		}
		m_tasks.emplace([task]() { (*task)(); });
		NLOG("FuncThreadPool::enqueue:tasks_size=%zu", m_tasks.size());
	}
	m_condition_variable.notify_one();
	return ret;
}

#define sFuncThreadPool (*FuncThreadPool::Instance())

#endif
