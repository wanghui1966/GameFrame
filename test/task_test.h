#ifndef TEST_TASK_H
#define TEST_TASK_H

#include "task.h"

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
        std::cout << m_task_test_id << " Start." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << m_task_test_id << " end." << std::endl;
    }

protected:
    uint32_t                m_task_test_id = 0;
};

#endif
