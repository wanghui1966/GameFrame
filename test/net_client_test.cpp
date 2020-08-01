#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include <thread>
#include <chrono>

#include "debug.h"
#include <cstring>

#include "packet.h"

const int MAX_BUF_LEN = 1024;	// buf长度
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666
void ClientFunc()
{
	// 创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		ELOG("ClientFunc fail for socket");
		return;
	}

	// 服务器套接字
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// 连接服务器
	if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		ELOG("ClientFunc fail for connect");
		return;
	}

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	getsockname(fd, (struct sockaddr*)&client_addr, &client_addr_len);
	char *client_ip = inet_ntoa(client_addr.sin_addr);
	in_port_t client_port = ntohs(client_addr.sin_port);

	char buf[MAX_BUF_LEN] = {0};

	int send_index = 0;
	while (true)
	{
		sleep(3);

		Data client_data('a', 'c', true, -2, 9, -98, 89, -4343423233343, 321298765678, 7.2, 8.9, "123456789abcdefghijklmnopqrstuvwxyz");

		// 组装包
		Packet client_packet(6666);
		client_data.ToPacket(client_packet);

		// 模拟发包之前打包
		client_packet.PackageData();

		write(fd, client_packet.GetData(), client_packet.GetSize());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		break;
	}

	close(fd);
}

int main()
{
	ClientFunc();

	return 0;
}
