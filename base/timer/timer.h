#pragma once

#ifndef TIMER_H
#define TIMER_H

#include "../thread_pool/task.h"
#include "../singleton.h"

#include <cassert>
#include <cstdint>
#include <signal.h>
#include <sys/time.h>
#include <map>

#define MICROSECONDS 1000000
#define DEFAULT_INTERVAL MICROSECONDS
// 根据延时时间计算延时tick
#define DELAY_TICK(second) ((second * MICROSECONDS) / DEFAULT_INTERVAL)

// 定时器
class Timer : public Singleton<Timer>
{
public:
	Timer() {}
	~Timer() {}

public:
	// interval为定时器间隔，单位为微秒(1秒=1000毫秒=1000000微秒)
	static bool StartTimer(int64_t interval = 0)
	{
		if (m_active)
		{
			return false;
		}
		m_active = true;
		m_interval = (interval > 0 ? interval : DEFAULT_INTERVAL);
		m_tick = 0;

		// 设置一个信号集合，设置SIGALRM为阻塞信号
		sigemptyset(&m_sigset);
		sigaddset(&m_sigset, SIGALRM);
		sigprocmask(SIG_BLOCK, &m_sigset, nullptr);

		// 注册定时器处理信号SIGALRM
		signal(SIGALRM, Update);

		// 设置定时器 轮询间隔和延时时长
		struct timeval tv;
		tv.tv_sec = m_interval / MICROSECONDS;
		tv.tv_usec = m_interval % MICROSECONDS;
		struct itimerval itv;
		itv.it_interval = tv;
		itv.it_value = tv;

		// 系统真实时间轮询触发SIGALRM信号
		setitimer(ITIMER_REAL, &itv, nullptr);

		// 放开阻塞
		sigprocmask(SIG_UNBLOCK, &m_sigset, nullptr);
		// 如果想实现Suspend和Resume功能
		// 在Suspend函数时候阻塞信号：sigprocmask(SIG_BLOCK, &m_sigset, nullptr);
		// 在Resume函数时放开阻塞：sigprocmask(SIG_UNBLOCK, &m_sigset, nullptr);

		return true;
	}

	static void StopTimer()
	{
		m_active = false;
	}

	static bool AddTask(int delay_tick, Task *task)
	{
		if (!m_active)
		{
			return false;
		}
		int64_t tick = m_tick + (delay_tick > 0 ? delay_tick : 0);

		m_timer_tasks.insert(std::make_pair(tick, task));
	}

	static void Update(int sig)
	{
		if (sig != SIGALRM)
		{
			return;
		}

		if (!m_active)
		{
			assert(false);
			return;
		}
		++m_tick;

		while (true)
		{
			auto it = m_timer_tasks.begin();
			if (it != m_timer_tasks.end() && it->first <= m_tick)
			{
				Task *cur_task = it->second;
				m_timer_tasks.erase(it);
				if (cur_task)
				{
					cur_task->Run();
				}
			}
			else
			{
				break;
			}
		}
	}

public:
	static bool									m_active;		// 是否已激活
	static int									m_interval;		// 轮询间隔
	static int64_t								m_tick;			// 轮询的次数
	static sigset_t								m_sigset;
	static std::multimap<int64_t, Task*>		m_timer_tasks;
};
bool Timer::m_active = false;
int Timer::m_interval = 0;
int64_t Timer::m_tick = 0;
sigset_t Timer::m_sigset;
std::multimap<int64_t, Task*> Timer::m_timer_tasks;
#define sTimer (*Timer::Instance())

#endif
