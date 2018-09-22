#ifndef TEST_TASK_H
#define TEST_TASK_H

#include "task.h"
#include "log.h"

#include <iostream>
#include <thread>
#include <chrono>

class TaskTest : public Task
{
public:
    TaskTest() : m_task_test_id(0) {}
    TaskTest(uint32_t id) : m_task_test_id(id) {}
    virtual ~TaskTest(){}
    
    virtual void Run()
    {
		NLOG("TaskTest::Run begin:task=%d", m_task_test_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		NLOG("TaskTest::Run end:task=%d", m_task_test_id);
    }

protected:
    uint32_t                m_task_test_id = 0;
};

#endif
