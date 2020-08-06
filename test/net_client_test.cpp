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
#include "common.pb.h"
#include "game_packet.h"

bool ReadByLen(int fd, char *buf, ssize_t need_read_len)
{
	ssize_t has_read_len = 0;
	while (true)
	{
		ssize_t len = read(fd, buf + has_read_len, need_read_len - has_read_len);
		has_read_len += len;
		if (has_read_len < need_read_len)
		{
			NLOG("ReadByLen continue:len=%d, has_read_len=%d, need_read_len=%d", len, has_read_len, need_read_len);
			continue;
		}
		else if (has_read_len == need_read_len)
		{
			break;
		}
		else if (has_read_len > need_read_len)
		{
			ELOG("ReadByLen fail:len=%d, has_read_len=%d, need_read_len=%d", len, has_read_len, need_read_len);
			return false;
		}
	}
	return true;
}

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

	int index = 0;
	while (true)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		{
			std::vector<int> v{1, 3, 5, 7, 9};
			TestData client_data('a', 'c', true, -2, 9, -98, 89, -4343423233343, 321298765678, 7.2, 8.9, "123456789abcdefghijklmnopqrstuvwxyz", v);

			// 组装包
			Packet client_packet(GPO_TEST_DATA);
			client_data.ToPacket(client_packet);

			// 模拟发包之前打包
			client_packet.PackageData();

			// write(fd, client_packet.GetData(), client_packet.GetSize());
			// 分两次发，测试服务器的合包
			char *buf = client_packet.GetData();
			uint32_t buf_size = client_packet.GetSize();
			write(fd, buf, 20);
			write(fd, buf + 20, buf_size - 20);

			std::string str;
			client_data.GetStr(str);
			NLOG("write data:opcode=%u, data_length=%u, data=[%s]", client_packet.GetOpcode(), client_packet.GetDataLength(), str.c_str());
		}
		{
			char buf[114] = {0};
			if (!ReadByLen(fd, buf, 114))
			{
				ELOG("read data fail");
				break;
			}
			uint32_t data_length = *((uint32_t*)buf);
			uint32_t opcode = *((uint32_t*)(buf + PACKET_DATA_LENGTH_SIZE));

			Packet rsp_packet(opcode, data_length);
			rsp_packet.UnpackageData(buf + PACKET_HEADER_SIZE, data_length);

			TestData data;
			data.PacketTo(rsp_packet);

			std::string str;
			data.GetStr(str);
			NLOG("read data:opcode=%u, data_length=%u, data=[%s]", opcode, data_length, str.c_str());
		}

		{
			PB::dt_get_common_info_req client_pb;
			client_pb.set_id(2);

			// 客户端数据
			std::string client_data = client_pb.SerializeAsString();
			uint32_t client_data_length = client_data.length();

			// 组装包
			Packet client_packet(PB::DT_GET_COMMON_INFO_REQ, client_data_length);
			client_packet.PushValue(client_data.c_str(), client_data_length);

			// 模拟发包之前打包
			client_packet.PackageData();

			// write(fd, client_packet.GetData(), client_packet.GetSize());
			// write(fd, client_packet.GetData(), client_packet.GetSize());
			// 发两个包，测试服务器的拆包
			char *packet_buf = client_packet.GetData();
			int packet_size = client_packet.GetSize();
			char *buf = new char[packet_size * 2];
			memset(buf, 0, packet_size * 2);
			memcpy(buf, packet_buf, packet_size);
			memcpy(buf + packet_size, packet_buf, packet_size);
			write(fd, buf, packet_size * 2);

			NLOG("write pb*2:opcode=%u, data_length=%u, data=[%s]", client_packet.GetOpcode(), client_packet.GetDataLength(), client_pb.DebugString().c_str());
		}
		{
			char buf[200] = {0};
			if (!ReadByLen(fd, buf, 200))
			{
				ELOG("read pb fail");
				break;
			}

			// 第一个包
			uint32_t data_length1 = *((uint32_t*)buf);
			uint32_t opcode1 = *((uint32_t*)(buf + PACKET_DATA_LENGTH_SIZE));
			PB::dt_get_common_info_rsp rsp_pb1;
			rsp_pb1.ParseFromString(buf + PACKET_HEADER_SIZE);
			NLOG("read pb1:opcode=%u, data_length=%u, data=[%s]", opcode1, data_length1, rsp_pb1.DebugString().c_str());

			// 第二个包
			uint32_t data_length2 = *((uint32_t*)(buf + 100));
			uint32_t opcode2 = *((uint32_t*)(buf + 100 + PACKET_DATA_LENGTH_SIZE));
			PB::dt_get_common_info_rsp rsp_pb2;
			rsp_pb2.ParseFromString(buf + 100 + PACKET_HEADER_SIZE);
			NLOG("read pb2:opcode=%u, data_length=%u, data=[%s]", opcode2, data_length2, rsp_pb2.DebugString().c_str());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		++index;
		if (index >= 10)
		{
			break;
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	close(fd);
}

int main()
{
	ClientFunc();

	return 0;
}
