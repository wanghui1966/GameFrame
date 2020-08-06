#include "timer.h"
#include "task.h"
#include "common.h"

#include <thread>
#include <chrono>

#include "debug.h"

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
		NLOG("CricleTaskTest::Run begin:task=%u", m_task_id);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		NLOG("CricleTaskTest::Run end:task=%u", m_task_id);
		circle_task_test(++m_task_id);
		delete this;
    }

protected:
    uint32_t                m_task_id = 0;
};
void circle_task_test(int task_id)
{
	sTimer.AddTask(2, new CircleTaskTest(task_id));
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
		NLOG("OnceTaskTest::Run begin:task=%u", m_task_id);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		NLOG("OnceTaskTest::Run end:task=%u", m_task_id);
		delete this;
	}

protected:
    uint32_t                m_task_id = 0;
};
void once_task_test(int task_id)
{
	sTimer.AddTask(5, new OnceTaskTest(task_id));
}

int main()
{
	sTimer.NewInstance();
	sTimer.StartTimer(1000000);

	// 此线程只是让主线程阻塞永远不结束而已
	std::thread th([&]{ circle_task_test(1); once_task_test(1);  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); } });
	th.join();

	sTimer.DeleteInstance();
	return 0;
}
