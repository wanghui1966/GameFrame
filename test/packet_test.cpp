#include "packet.h"

#include <string>
#include <cstring>
#include "../protobuf/cpp/common.pb.h"

#include <cstdio>

void test_struct()
{
	printf("test_struct begin\n");

	struct Data
	{
		char				data1 = 0;
		unsigned char		data2 = 0;
		bool				data3 = false;
		short				data4 = 0;
		unsigned short		data5 = 0;
		int32_t				data6 = 0;
		uint32_t			data7 = 0;
		int64_t				data8 = 0;
		uint64_t			data9 = 0;
		float				data10 = 0;
		double				data11 = 0;
		std::string			data12;
	
		Data() {}
		Data(char d1, unsigned char d2, bool d3, short d4, unsigned short d5, int32_t d6, uint32_t d7, int64_t d8, uint64_t d9, float d10, double d11, const std::string &d12) : data1(d1), data2(d2), data3(d3), data4(d4), data5(d5), data6(d6), data7(d7), data8(d8), data9(d9), data10(d10), data11(d11), data12(d12) {}
	
		void ToPacket(Packet &p)
		{
			p << data1 << data2 << data3 << data4 << data5 << data6 << data7 << data8 << data9 << data10 << data11 << data12;
		}
	
		void PacketTo(Packet &p)
		{
			p >> data1 >> data2 >> data3 >> data4 >> data5 >> data6 >> data7 >> data8 >> data9 >> data10 >> data11 >> data12;
		}
	
		void GetStr(std::string &str)
		{
			char buf[1024] = {0};
			snprintf(buf, sizeof(buf), "data1=%c, data2=%c, data3=%d, data4=%hd, data5=%hu, data6=%d, data7=%u, data8=%lld, data9=%llu, data10=%.2f, data11=%.2lf, data12=%s\n", data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12.c_str());
			str = buf;
		}
	};

	// 客户端数据
	Data client_data('a', 'c', true, -2, 9, -98, 89, -4343423233343, 321298765678, 7.2, 8.9, "123456789abcdefghijklmnopqrstuvwxyz");

	// 组装包
	Packet client_packet(6666);
	client_data.ToPacket(client_packet);

	// 模拟发包之前打包
	client_packet.PackageData();

	std::string client_data_str;
	client_data.GetStr(client_data_str);
	printf("client_data:opcode=%u, data_length=%u, data=[%s]\n", client_packet.GetOpcode(), client_packet.GetDataLength(), client_data_str.c_str());

	// 模拟收包之后得到buf
	uint32_t buf_size = client_packet.GetSize();
	char *buf = new char[buf_size];
	memset(buf, 0, sizeof(buf));
	memcpy(buf, client_packet.GetData(), buf_size);
	
	// 服务器数据
	Data server_data;

	// 解包
	Packet server_packet(6666, client_packet.GetDataLength());
	server_packet.UnpackageData(buf, buf_size);
	server_data.PacketTo(server_packet);

	std::string server_data_str;
	server_data.GetStr(server_data_str);
	printf("server_data:opcode=%u, data_length=%u, data=[%s]\n", server_packet.GetOpcode(), server_packet.GetDataLength(), server_data_str.c_str());
	printf("test_struct end\n\n");
}

void GetPBStr(const PB::common_info& pb, std::string &str)
{
	char buf[256] = {0};
	snprintf(buf, sizeof(buf), "flag=%d, str=%s, ri_size=%d, ri=(", pb.flag(), pb.str().c_str(), pb.ri_size());
	str = buf;

	for (int i = 0; i < pb.ri_size(); ++i)
	{
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "%lld, ", pb.ri(i));
		str += buf;
	}
	str += ")";
}
void test_pb()
{
	printf("test_pb begin\n");

	PB::common_info client_pb;
	client_pb.set_flag(true);
	client_pb.set_str(std::string("测试optional数据"));
	client_pb.add_ri(1);
	client_pb.add_ri(9);
	client_pb.add_ri(19);

	// 客户端数据
	std::string client_data = client_pb.SerializeAsString();
	uint32_t client_data_length = client_data.length();

	// 组装包
	Packet client_packet(7777, client_data_length);
	client_packet.PushValue(client_data.c_str(), client_data_length);

	// 模拟发包之前打包
	client_packet.PackageData();

	std::string client_data_str;
	GetPBStr(client_pb, client_data_str);
	printf("client_data:opcode=%u, data_length=%u, data=[%s]\n", client_packet.GetOpcode(), client_packet.GetDataLength(), client_data_str.c_str());

	// 模拟收包之后得到buf
	uint32_t buf_size = client_packet.GetSize();
	char *buf = new char[buf_size];
	memset(buf, 0, sizeof(buf));
	memcpy(buf, client_packet.GetData(), buf_size);
	
	// 服务器数据
	uint32_t server_data_length = client_packet.GetDataLength();
	char *server_data = new char[buf_size + 1];
	memset(server_data, 0, buf_size + 1);

	// 解包
	Packet server_packet(6666, server_data_length);
	server_packet.UnpackageData(buf, buf_size);
	server_packet.PopValue(server_data, server_data_length);

	PB::common_info server_pb;
	server_pb.ParseFromString(server_data);

	std::string server_data_str;
	GetPBStr(server_pb, server_data_str);
	printf("server_data:opcode=%u, data_length=%u, data=[%s]\n", server_packet.GetOpcode(), server_packet.GetDataLength(), server_data_str.c_str());
	printf("test_pb end\n\n");
}

int main()
{
	test_struct();
	test_pb();
	return 0;
}
