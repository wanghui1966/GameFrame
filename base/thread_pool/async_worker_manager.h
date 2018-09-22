#ifndef ASYNC_WORKER_MANAGER
#define ASYNC_WORKER_MANAGER

#include "../singleton.h"
#include "thread_pool.h"

#define DEFAULT_THREAD_POOL_SIZE 10

class AsyncWorkerManager : public ThreadPool, public Singleton<AsyncWorkerManager>
{
public:
    explicit AsyncWorkerManager(uint32_t pool_size = DEFAULT_THREAD_POOL_SIZE) : ThreadPool(pool_size) {}
};

#define sAsyncWorkerManager (*AsyncWorkerManager::Instance())

#endif
