#include "async_worker_manager.h"
#include "task_test.h"

#include <cstdio>
#include <thread>

int main()
{
	sLog.NewInstance();
	sLog.Init("../config/log.properties");
	std::thread thlog([=]{ while(true) {sLog.Update();} });
	thlog.detach();

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

	sLog.DeleteInstance();
    return 0;
}
