#include "timer.h"
#include "../thread_pool/task.h"
#include "../common.h"

#include <iostream>
#include <thread>
#include <chrono>

// 循环执行的任务，执行完任务后2个tick后再执行一遍
void circle_task_test(int task_id);
class CircleTaskTest : public Task
{
public:
    CircleTaskTest() : m_task_id(0) {}
    CircleTaskTest(uint32_t task_id) : m_task_id(task_id) {}
    virtual ~CircleTaskTest(){}
    
    virtual void Run()
    {
		std::cout << GetTimeStr(time(nullptr)) << " CricleTaskTest::Run begin:task=" << m_task_id << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		std::cout << GetTimeStr(time(nullptr)) << " CircleTaskTest::Run end:task=" << m_task_id << std::endl;
		circle_task_test(++m_task_id);
		delete this;
    }

protected:
    uint32_t                m_task_id = 0;
};
void circle_task_test(int task_id)
{
	iTimer.AddTask(2, new CircleTaskTest(task_id));
}

// 延时任务，延时5个tick执行一次任务
void once_task_test(int task_id);
class OnceTaskTest : public Task
{
public:
	OnceTaskTest(): m_task_id(0) {}
	OnceTaskTest(uint32_t task_id) : m_task_id(task_id) {}
	virtual ~OnceTaskTest() {}

	virtual void Run()
	{
		std::cout << GetTimeStr(time(nullptr)) << " OnceTaskTest::Run begin:task=" << m_task_id << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << GetTimeStr(time(nullptr)) << " OnceTaskTest::Run end:task=" << m_task_id << std::endl;
		delete this;
	}

protected:
    uint32_t                m_task_id = 0;
};
void once_task_test(int task_id)
{
	iTimer.AddTask(5, new OnceTaskTest(task_id));
}

int main()
{
	iTimer.NewInstance();
	iTimer.StartTimer(1000000);

	// 此线程只是让主线程阻塞永远不结束而已
	std::thread th([&]{ circle_task_test(1); once_task_test(1);  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); } });
	th.join();

	iTimer.DeleteInstance();
	return 0;
}
