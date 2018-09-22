#include "ts_queue.h"

#include <iostream>


void TSQueueOutput(TSQueue<int>& tsq)
{
	bool is_empty = tsq.Empty();
	size_t size = tsq.Size();

	int front = 0;
	bool is_front = tsq.Front(front);

	int back = 0;
	bool is_back = tsq.Back(back);

	std::cout << "TSQueueOutput::is_empty=" << is_empty << ", size=" << size << ", is_front=" << is_front << ", front=" << front << ", is_back" << is_back << ", back=" << back << std::endl;
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
	std::cout << "pop_ret=" << pop_ret << ", tmp=" << tmp << std::endl;
	TSQueueOutput(tsq);
}

int main()
{
	TSQueueTest();	
	return 0;
}
