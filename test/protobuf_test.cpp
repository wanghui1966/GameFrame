#include <cstdio>
#include <string>

#include "../protobuf/cpp/common.pb.h"

#include "debug.h"

void FillData(PB::dt_get_common_info_rsp &rsp)
{
	rsp.set_id(1);

	PB::common_info ci;
	ci.set_flag(true);
	ci.set_str(std::string("测试optional数据"));
	ci.add_ri(1);
	rsp.mutable_oci()->CopyFrom(ci);

	for (int i = 0; i < 2; ++i)
	{
		PB::common_info* ele = rsp.add_rci();
		ele->set_flag(i / 2 == 0);
		ele->set_str(std::string("测试repeated数据").append(std::to_string(i)));
		ele->add_ri(i);
		ele->add_ri(i + 1);
	}
}

int main()
{
	PB::dt_get_common_info_rsp origin_rsp;
	// 填充数据
	FillData(origin_rsp);
	// 序列化
	std::string str = origin_rsp.SerializeAsString();
	NLOG("origin_rsp=%s", origin_rsp.DebugString().c_str());

	PB::dt_get_common_info_rsp rsp;
	// 反序列化
	rsp.ParseFromString(str);
	NLOG("rsp=%s", origin_rsp.DebugString().c_str());

	return 0;
}
