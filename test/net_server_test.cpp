#include "net_manager.h"

#include <thread>
#include <chrono>

int main()
{
    sNetManager.NewInstance();
	sNetManager.Init();
	sNetManager.Open();
    std::thread th([=]{ while(true) { sNetManager.Heartbeat(); std::this_thread::sleep_for(std::chrono::milliseconds(1000)); } });
    th.join();

    sNetManager.DeleteInstance();
	return 0;
}
