#include "patch_wind.h"
#include "patch.h"

#include <thread>
#include <chrono>

#include "debug.h"

int main()
{
	sPatch.NewInstance();
	sPatch.Init("./libpatch.so", 5);

	PatchWind w(-9);
	while (true)
	{
		NLOG("begin======");

		int i_sum = GlobalFunc(1, 1);
		NLOG("GlobalFunc:%d", i_sum);

		i_sum = PatchWind::StaticFunc(2, 2);
		NLOG("StaticFunc:%d", i_sum);

		double d_sum = w.MemberFunc(3, 3);
		NLOG("MemberFunc:%f", d_sum);

		NLOG("end======\n");

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		sPatch.Update();
	}

	sPatch.DeleteInstance();
	return 1;
}
