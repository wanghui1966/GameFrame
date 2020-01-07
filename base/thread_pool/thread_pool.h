#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <mutex>
#include <vector>
#include <map>
#include <queue>
#include <cstdint>

class Task;
class Worker;

class ThreadPool
{
public:
	explicit ThreadPool(uint32_t pool_size);
	virtual ~ThreadPool();

public:
	bool Init();
	void Start();
	void Stop();
	void AddWork(Task *task);
	void Update();

protected:
	void ReleaseWorkers();
	void ReleaseTasks();

protected:
	std::mutex								m_mutex;				// 互斥锁
	uint32_t								m_pool_size;			// 线程池中线程数量
	std::vector<Worker*>					m_workers;				// 工作线程列表
	uint32_t								m_task_index;			// 任务索引
	std::map<uint32_t, std::queue<Task*>>	m_tasks;				// 任务列表
	uint32_t								m_last_update_time;		// 上次update时间
};

#endif
