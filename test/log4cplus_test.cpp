#include "log.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <thread>

int main()
{
    sLog.NewInstance();
    sLog.Init("../config/log.properties");

    std::thread th([=]{ while(true) {sLog.Update();} });
    th.detach();

	while (true)
	{
		int rand_num = rand() % 1024;
		int mod_num = rand_num % 5;
		switch (mod_num)
		{
		case 0:
			FLOG("rand_num=%d, mod_num=%d", rand_num, mod_num);
			break;
		case 1:
			ELOG("rand_num=%d, mod_num=%d", rand_num, mod_num);
			break;
		case 2:
			WLOG("rand_num=%d, mod_num=%d", rand_num, mod_num);
			break;
		case 3:
			NLOG("rand_num=%d, mod_num=%d", rand_num, mod_num);
			break;
		case 4:
			DLOG("rand_num=%d, mod_num=%d", rand_num, mod_num);
			break;
		default:
			break;
		}
		usleep(rand() % 100000 + 1);
	}

    sLog.DeleteInstance();
    return 0;
}
