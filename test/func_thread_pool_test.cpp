#include "func_thread_pool.h"
#include "debug.h"

#include <iostream>

int main()
{
	sFuncThreadPool.NewInstance();
	sFuncThreadPool.Init(4);

	std::vector<std::future<int>> results;
	results.reserve(8);
	for (int i = 0; i < 8; ++i)
	{
		results.emplace_back(sFuncThreadPool.enqueue([i]
					{
						std::cout << "task_" << i << " begin." << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						std::cout << "task_" << i << " end." << std::endl;
						return i * i;
					}
				));
	}
	for (auto &&result : results)
	{
		NLOG("result=%d.", result.get());
	}

	sFuncThreadPool.Stop();
	sFuncThreadPool.DeleteInstance();
	return 0;
}
