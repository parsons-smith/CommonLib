#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include "Condition.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>

void *thread_routine(void *args);
class ThreadPool
{
	friend void *thread_routine(void *args);
private:
	//回调函数类型
	typedef void *(*callback_t)(void *);
	//任务结构体
	struct task_t
	{
		callback_t run; //任务回调函数
		void *args;	 //任务函数参数
	};

public:
	ThreadPool(int _maxThreads = 36, unsigned int _waitSeconds = 2);
	~ThreadPool();
	//添加任务接口
	void addTask(callback_t run, void *args);

private:
	void startTask();

private:
	Condition ready;				//任务准备就绪或线程池销毁通知
	std::queue<task_t *> taskQueue; //任务队列

	unsigned int maxThreads;		//线程池最多允许的线程数
	unsigned int counter;		   //线程池当前线程数
	unsigned int idle;			  //线程池空闲线程数
	unsigned int waitSeconds;	   //线程可以等待的秒数
	bool		 quit;			  //线程池销毁标志
};

#endif // THREADPOOL_H_INCLUDED
