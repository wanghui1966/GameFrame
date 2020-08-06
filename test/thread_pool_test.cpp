#include "async_worker_manager.h"
#include "task.h"

#include <cstdio>
#include <thread>
#include <chrono>

#include "debug.h"

class TaskTest : public Task
{
public:
	TaskTest() : m_task_test_id(0) {}
	TaskTest(uint32_t id) : m_task_test_id(id) {}
	virtual ~TaskTest(){}

	virtual void Run()
	{
		NLOG("TaskTest::Run begin:task=%d", m_task_test_id);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		NLOG("TaskTest::Run end:task=%d", m_task_test_id);
	}

protected:
	uint32_t				m_task_test_id = 0;
};

int main()
{
	sAsyncWorkerManager.NewInstance();
	sAsyncWorkerManager.Init();
	sAsyncWorkerManager.Start();

	std::thread th([=]{ while(true) {sAsyncWorkerManager.Update();} });
	th.detach();

	int index = 0;
	int flag = 0;
	while (scanf("%d", &flag) > 0)
	{
		sAsyncWorkerManager.AddWork(new TaskTest(index++));
		fflush(stdin);
	}

	sAsyncWorkerManager.Stop();
	sAsyncWorkerManager.DeleteInstance();

	return 0;
}
