#include "net_manager.h"
#include "protobuf_factory.h"
#include "game_command.h"

#include <thread>
#include <chrono>

int main()
{
	sProtobufFactory.NewInstance();
	sProtobufFactory.Init();

	sGameCommand.NewInstance();

	sNetManager.NewInstance();
	sNetManager.Init();
	sNetManager.Open();

	std::thread th([=]{ while(true) { sNetManager.Heartbeat(); std::this_thread::sleep_for(std::chrono::milliseconds(2000)); } });
	th.join();

	sNetManager.DeleteInstance();
	sGameCommand.DeleteInstance();
	sProtobufFactory.DeleteInstance();
	return 0;
}
