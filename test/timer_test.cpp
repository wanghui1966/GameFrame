#include "timer.h"
#include "../thread_pool/task.h"
#include "../common.h"

#include <iostream>
#include <thread>
#include <chrono>

void task_test(int task_test_id);
class TaskTest : public Task
{
public:
    TaskTest() : m_task_test_id(0) {}
    TaskTest(uint32_t id) : m_task_test_id(id) {}
    virtual ~TaskTest(){}
    
    virtual void Run()
    {
		std::cout << GetTimeStr(time(nullptr)) << " TaskTest::Run begin:task=" << m_task_test_id << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::this_thread::sleep_for(std::chrono::microseconds(500000));
		std::cout << GetTimeStr(time(nullptr)) << " TaskTest::Run end:task=" << m_task_test_id << std::endl;
		task_test(++m_task_test_id);
		delete this;
    }

protected:
    uint32_t                m_task_test_id = 0;
};

void task_test(int task_test_id)
{
	iTimer.AddTask(1, new TaskTest(task_test_id));
}

int main()
{
	iTimer.NewInstance();
	iTimer.StartTimer(1000000);

	task_test(1);

	std::this_thread::sleep_for(std::chrono::seconds(6000));

	iTimer.DeleteInstance();
	return 0;
}
