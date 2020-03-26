#include <cstdio>
#include <string>

#include "../protobuf/cpp/common.pb.h"

void PackRsp(PB::dt_get_common_info_rsp &rsp)
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

void UnpackRsp(const PB::dt_get_common_info_rsp &rsp)
{
	printf("UnpackRsp begin\n\n");
	printf("type=%d\n", rsp.type());
	printf("id=%d\n", rsp.id());

	const PB::common_info &ci = rsp.oci();
	printf("oci=(flag=%d, str=%s, ri_size=%d", ci.flag(), ci.str().c_str(), ci.ri_size());
	for (int i = 0; i < ci.ri_size(); ++i)
	{
		printf(", ri_%d=%ld", i, ci.ri(i));
	}
	printf(")\n");

	printf("rci=(rci_size=%d", rsp.rci_size());
	for (int i = 0; i < rsp.rci_size(); ++i)
	{
		const PB::common_info &ele = rsp.rci(i);
		printf(", rci_%d=[flag=%d, str=%s, ri_size=%d", i, ele.flag(), ele.str().c_str(), ele.ri_size());
		for (int j = 0; j < ele.ri_size(); ++j)
		{
			printf(", ri_%d=%ld", j, ele.ri(j));
		}
		printf("]");
	}
	printf(")\n");

	printf("\nUnpackRsp end\n");
}

int main()
{
	PB::dt_get_common_info_rsp origin_rsp;
	PackRsp(origin_rsp);
	std::string str = origin_rsp.SerializeAsString();

	PB::dt_get_common_info_rsp rsp;
	rsp.ParseFromString(str);
	UnpackRsp(rsp);

	return 0;
}
