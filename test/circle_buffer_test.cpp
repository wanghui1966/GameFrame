#include "circle_buffer.h"

#include "debug.h"
#include <cstring>

void OutputInfo(const CircleBuffer &cb)
{
	NLOG("CircleBuffer:buf_capacity=%d, can_write_size=%d, can_read_size=%d\n", cb.GetCapacity(), cb.GetCanWriteSize(), cb.GetCanReadSize());
}

int main()
{
	CircleBuffer cb(10);
	OutputInfo(cb);

	std::string str1 = "123456";
	cb.Write(str1.c_str(), str1.size());
	OutputInfo(cb);

	char buf[10] = {0};
	cb.Read(buf, 9);
	OutputInfo(cb);
	NLOG("read buf=%s", buf);

	cb.Read(buf, 2);
	OutputInfo(cb);
	NLOG("read buf=%s", buf);

	std::string str2 = "678901";
	cb.Write(str2.c_str(), str2.size());
	OutputInfo(cb);

	std::string str3 = "6789";
	cb.Write(str3.c_str(), str3.size());
	OutputInfo(cb);

	cb.Read(buf, 8);
	OutputInfo(cb);
	NLOG("read buf=%s", buf);

	return 0;
}
