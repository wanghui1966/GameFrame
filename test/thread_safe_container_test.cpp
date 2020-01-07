#include "ts_queue.h"

#include "debug.h"

void TSQueueOutput(TSQueue<int>& tsq)
{
	bool is_empty = tsq.Empty();
	size_t size = tsq.Size();

	int front = 0;
	bool is_front = tsq.Front(front);

	int back = 0;
	bool is_back = tsq.Back(back);

	NLOG("TSQueueOutput::is_empty=%d, size=%zu, is_front=%d, front=%d, is_back=%d, back=%d", is_empty, size, is_front, front, is_back, back);
}
void TSQueueTest()
{
	TSQueue<int> tsq;
	tsq.Push(1);
	tsq.Push(2);
	tsq.Push(3);
	TSQueueOutput(tsq);

	int tmp = 0;
	bool pop_ret = tsq.Pop(tmp);
	NLOG("pop_ret=%d, tmp=%d", pop_ret, tmp);
	TSQueueOutput(tsq);
}

int main()
{
	TSQueueTest();	
	return 0;
}
