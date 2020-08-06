#pragma once

#ifndef TASK_H
#define TASK_H

/****************************************************/
/* 任务类											*/
/* 需要在线程池中运行的任务均应从此类中继承			*/
/****************************************************/
class Task
{
public:
	Task(){}
	virtual ~Task(){}
	virtual void Run() = 0;
};

#endif
