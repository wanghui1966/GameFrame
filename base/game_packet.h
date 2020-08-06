#pragma once

#ifndef GAME_PACKET_H
#define GAME_PACKET_H

#include "packet/packet.h"
#include "common_string.h"

#include <string>
#include <vector>

enum GAME_PACKET_OPCODE
{
	GPO_NONE						= 0,
	GPO_TEST_DATA					= 1,

	// PB协议
	GPO_PB_BEGIN					= 10000,
	GPO_PB_END						= 20000,
};

struct TestData
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
	std::vector<int>	data13;

	TestData() {}
	TestData(char d1, unsigned char d2, bool d3, short d4, unsigned short d5, int32_t d6, uint32_t d7, int64_t d8, uint64_t d9, float d10, double d11, const std::string &d12, std::vector<int> &d13) : data1(d1), data2(d2), data3(d3), data4(d4), data5(d5), data6(d6), data7(d7), data8(d8), data9(d9), data10(d10), data11(d11), data12(d12)
	{
		data13.swap(d13);
	}

	void ToPacket(Packet &p)
	{
		p << data1 << data2 << data3 << data4 << data5 << data6 << data7 << data8 << data9 << data10 << data11 << data12;
		int data13_size = data13.size();
		p << data13_size;
		for (auto it = data13.begin(); it != data13.end(); ++it)
		{
			p << *it;
		}
	}

	void PacketTo(Packet &p)
	{
		p >> data1 >> data2 >> data3 >> data4 >> data5 >> data6 >> data7 >> data8 >> data9 >> data10 >> data11 >> data12;
		int data13_size;
		p >> data13_size;
		for (int i = 0; i < data13_size; ++i)
		{
			int data13_data = 0;
			p >> data13_data;
			data13.push_back(data13_data);
		}
	}

	void GetStr(std::string &str)
	{
		std::string data13_str;
		StringHelper::GetVectorStr<int>(data13, data13_str);

		char buf[1024] = {0};
		snprintf(buf, sizeof(buf), "data1=%c, data2=%c, data3=%d, data4=%hd, data5=%hu, data6=%d, data7=%u, data8=%lld, data9=%llu, data10=%.2f, data11=%.2lf, data12=%s, data13=%s", data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12.c_str(), data13_str.c_str());
		str = buf;
	}
};

#endif
