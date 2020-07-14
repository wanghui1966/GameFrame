#include "worker.h"
#include "task.h"

Worker::Worker(Task *task/* = nullptr*/)
: m_task(task)
, m_running(false)
, m_thread(nullptr)
{

}

Worker::~Worker()
{
	if (m_thread != nullptr)
	{
		delete m_thread;
		m_thread = nullptr;
	}
}

void Worker::SetTask(Task *task)
{
	if (task == nullptr)
	{
		return;
	}

	std::lock_guard<std::mutex> lck(m_mutex);
	m_task = task;
	m_cv.notify_one();
}

bool Worker::IsRunning()
{
	return m_running;
}

void Worker::Run()
{
	if (m_running)
	{
		return;
	}
	m_running = true;

	while (m_running)
	{
		Task *task = nullptr;
		if ((task = m_task) == nullptr)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_cv.wait_for(lck, std::chrono::milliseconds(100));
			continue;
		}

		task->Run();
		delete task;
		m_task = nullptr;
	}
}

void Worker::Stop()
{
	if (m_running == false || m_thread == nullptr)
	{
		return;
	}

	m_running = false;
	m_cv.notify_one();

	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}
