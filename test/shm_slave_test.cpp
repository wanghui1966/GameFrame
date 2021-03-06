#include "common.h"
#include "shm_manager.h"

#include <cassert>
#include <ctime>
#include <thread>
#include <chrono>

int main()
{
	sShmManager.NewInstance();
	if (!sShmManager.Attach())
	{
		assert(false);
	}
	ShmData *sd = sShmManager.GetShmData();
	if (!sd)
	{
		assert(false);
	}

	auto shm_op = [&sd]()
	{
		// 1. 测试数据
		std::lock_guard<std::mutex> lck(sd->m_common_mutex);
		NLOG("shm_slave_test begin:common_data=%d", sd->m_common_data);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		sd->m_common_data = 2;
		NLOG("shm_slave_test end");
	};
	while (true)
	{
		std::thread th(shm_op);
		th.join();
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	
	sShmManager.Deattach();
	sShmManager.DeleteInstance();
	return 0;
}
