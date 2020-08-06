#include <cstdio>
#include <string>

#include "common.pb.h"
#include "protobuf_factory.h"

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
	{
		PB::dt_get_common_info_rsp origin_rsp;
		// 填充数据
		FillData(origin_rsp);
		// 序列化
		std::string str = origin_rsp.SerializeAsString();

		PB::dt_get_common_info_rsp rsp;
		// 反序列化
		rsp.ParseFromString(str);

		NLOG("origin_rsp=%s, rsp=%s", origin_rsp.DebugString().c_str(), rsp.DebugString().c_str());
	}

	{
		sProtobufFactory.NewInstance();
		sProtobufFactory.Init();

		::google::protobuf::Message *origin_rsp_message = sProtobufFactory.GetMessage(PT_COMMON, (int)PB::DT_GET_COMMON_INFO_RSP);
		PB::dt_get_common_info_rsp *origin_rsp = dynamic_cast<PB::dt_get_common_info_rsp*>(origin_rsp_message);
		// 填充数据
		FillData(*origin_rsp);
		// 序列化
		std::string str = origin_rsp->SerializeAsString();

		::google::protobuf::Message *rsp_message = sProtobufFactory.GetMessage(PT_COMMON, (int)PB::DT_GET_COMMON_INFO_RSP);
		// 反序列化
		rsp_message->ParseFromString(str);

		PB::dt_get_common_info_rsp *rsp = dynamic_cast<PB::dt_get_common_info_rsp*>(rsp_message);
		NLOG("sProtobufFactory::origin_rsp=%s, rsp=%s", origin_rsp->DebugString().c_str(), rsp->DebugString().c_str());

		sProtobufFactory.DeleteInstance();
	}
	return 0;
}
