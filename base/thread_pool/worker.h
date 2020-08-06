#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Task;

/****************************************************/
/* 工作线程类										*/
/* 绑定一个线程，并持有一个任务的指针				*/
/* 当没有任务时，线程就wait，等待notify				*/
/****************************************************/
class Worker
{
friend class ThreadPool;
public:
	explicit Worker(Task *task = nullptr);
	~Worker();

public:
	void SetTask(Task *task);
	bool IsRunning();
	void Run();
	void Stop();

protected:
	Task						*m_task;			// 任务指针
	bool						m_running;			// 是否正在运行
	std::thread					*m_thread;			// 线程指针
	std::condition_variable		m_cv;				// 条件变量
	std::mutex					m_mutex;			// 互斥锁
};

#endif
