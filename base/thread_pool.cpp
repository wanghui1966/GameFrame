#include "thread_pool.h"
#include "task.h"
#include "worker.h"

#include <iostream>

ThreadPool::ThreadPool(uint32_t pool_size)
: m_pool_size(pool_size)
, m_task_index(0)
, m_last_update_time(0)
{

}

ThreadPool::~ThreadPool() 
{
    ReleaseWorkers();
    ReleaseTasks();
}

bool ThreadPool::Init()
{
    for (uint32_t i = 0; i < m_pool_size; ++i)
    {
        Worker *worker = new Worker(nullptr);
        m_workers.push_back(worker);
    }
    return true;
}


void ThreadPool::Start()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    for (auto it = m_workers.begin(); it != m_workers.end(); ++it)
    {
        Worker *worker = *it;
        if (worker == nullptr)
        {
            continue;
        }
        worker->m_thread = new std::thread([=]{worker->Run();});
    }
}


void ThreadPool::Stop()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    for (auto it = m_workers.begin(); it != m_workers.end(); ++it)
    {
        Worker *worker = *it;
        if (worker == nullptr)
        {
            continue;
        }
        worker->Stop();
    }
}

void ThreadPool::AddWork(Task *task)
{
    if (task == nullptr)
    {
        return;
    }

    std::lock_guard<std::mutex> lck(m_mutex);
    std::queue<Task*> &tasks = m_tasks[m_task_index];
    std::cout << "ThreadPool::AddWork success:task_index=" << m_task_index << ", task_queue_size=" << tasks.size()  << std::endl;
    tasks.push(task);
    ++m_task_index;
    m_task_index = m_task_index % m_pool_size;
}

void ThreadPool::Update()
{
    uint32_t now_time = time(nullptr);
    if (now_time - m_last_update_time < 1)
    {
        return;
    }
    m_last_update_time = now_time;

    if (m_tasks.empty() || m_workers.empty())
    {
        return;
    }

    std::lock_guard<std::mutex> lck(m_mutex);
    int workers_size = m_workers.size();
    for (int i = 0; i < workers_size; ++i)
    {
        Worker *worker = m_workers[i];
        if (worker == nullptr)
        {
            continue;
        }
        std::queue<Task*> &tasks = m_tasks[i];
        if (tasks.empty())
        {
            continue;
        }

        Task *task = tasks.front();
        tasks.pop();
        worker->SetTask(task);
    }
}

void ThreadPool::ReleaseWorkers()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    for (auto it = m_workers.begin(); it != m_workers.end(); ++it)
    {
        Worker *&worker = *it;
        if (worker == nullptr)
        {
            continue;
        }
        if (worker->IsRunning())
        {
            worker->Stop();
        }
        delete worker;
        worker = nullptr;
    }
    m_workers.clear();
}

void ThreadPool::ReleaseTasks()
{
    for (auto it_i = m_tasks.begin(); it_i != m_tasks.end(); ++it_i)
    {
        if (it_i->second.empty())
        {
            continue;
        }

        while(!it_i->second.empty())
        {
            Task *&task = it_i->second.front();
            delete task;
            it_i->second.pop();
        }
    }
    m_tasks.clear();
}
