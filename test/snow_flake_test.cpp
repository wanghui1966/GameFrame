#include "snow_flake.h"

#include <thread>
#include <chrono>
#include "debug.h"

const int LOCAL_DATA_CENTER_ID = 6666;
const int LOCAL_MACHINE_ID = 8888;

int main()
{
	SnowFlake sf;
	sf.Init(LOCAL_DATA_CENTER_ID, LOCAL_MACHINE_ID);

	int count = 50;
	while (count)
	{
		NLOG("id=%ld", sf.GetNextId());
		--count;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
