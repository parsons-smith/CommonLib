#include "ThreadPool.h"

ThreadPool::ThreadPool(int _maxThreads, unsigned int _waitSeconds)
	: maxThreads(_maxThreads), counter(0), idle(0),
	  waitSeconds(_waitSeconds), quit(false) {}

// 线程入口
// 这其实就相当于一个消费者线程, 不断的消费任务(执行任务)
void *thread_routine(void *args)
{
	//将子线程设置成为分离状态, 这样主线程就可以不用jion
	pthread_detach(pthread_self());
	printf("*thread 0x%lx is starting...\n", (unsigned long)pthread_self());
	ThreadPool *pool = (ThreadPool *)args;

	//等待任务的到来, 然后执行任务
	while (true)
	{
		bool timeout = false;

		pool->ready.lock();
		//当处于等待的时候, 则说明空闲的线程多了一个
		++ pool->idle;

		//pool->ready中的条件变量有三个作用:
		// 1.等待任务队列中有任务到来
		// 2.等待线程池销毁通知
		// 3.确保当等待超时的时候, 能够将线程销毁(线程退出)
		while (pool->taskQueue.empty() && pool->quit == false)
		{
			printf("thread 0x%lx is waiting...\n", (unsigned long)pthread_self());
			//等待waitSeconds
			if (0 != pool->ready.timedwait(pool->waitSeconds))
			{
				//如果等待超时
				printf("thread 0x%lx is wait timeout ...\n", (unsigned long)pthread_self());
				timeout = true;
				//break出循环, 继续向下执行, 会执行到下面第1个if处
				break;
			}
		}
		//条件成熟(当等待结束), 线程开始执行任务或者是线程销毁, 则说明空闲线程又少了一个
		-- pool->idle;

		// 状态3.如果等待超时(一般此时任务队列已经空了)
		if (timeout == true && pool->taskQueue.empty())
		{
			-- pool->counter;
			//解锁然后跳出循环, 直接销毁线程(退出线程)
			pool->ready.unlock();
			break;
		}

		// 状态2.如果是等待到了线程的销毁通知, 且任务都执行完毕了
		if (pool->quit == true && pool->taskQueue.empty())
		{
			-- pool->counter;
			//如果没有线程了, 则给线程池发送通知
			//告诉线程池, 池中已经没有线程了
			if (pool->counter == 0)
				pool->ready.signal();
			//解锁然后跳出循环
			pool->ready.unlock();
			break;
		}

		// 状态1.如果是有任务了, 则执行任务
		if (!(pool->taskQueue.empty()))
		{
			//从队头取出任务进行处理
			ThreadPool::task_t *t = pool->taskQueue.front();
			pool->taskQueue.pop();

			//执行任务需要一定的时间
			//解锁以便于其他的生产者可以继续生产任务, 其他的消费者也可以消费任务
			pool->ready.unlock();
			//处理任务
			t->run(t->args);
			delete t;
		}
	}

	//跳出循环之后, 打印退出信息, 然后销毁线程
	printf("thread 0x%lx is exiting...\n", (unsigned long)pthread_self());
	pthread_exit(NULL);
}

//添加任务函数, 类似于一个生产者, 不断的将任务生成, 挂接到任务队列上, 等待消费者线程进行消费
void ThreadPool::addTask(callback_t run, void *args)
{
	/** 1. 生成任务并将任务添加到"任务队列"队尾 **/
	task_t *newTask = new task_t {run, args};

	ready.lock();   //注意需要使用互斥量保护共享变量
	taskQueue.push(newTask);

	/** 2. 让线程开始执行任务 **/
	startTask();
	ready.unlock();//解锁以使任务开始执行
}

//线程启动函数
void ThreadPool::startTask()
{
	// 如果有等待线程, 则唤醒其中一个, 让它来执行任务
	if (idle > 0)
		ready.signal();
	// 没有等待线程, 而且当前先线程总数尚未达到阈值, 我们就需要创建一个新的线程
	else if (counter < maxThreads)
	{
		pthread_t tid;
		pthread_create(&tid, NULL, thread_routine, this);
		++ counter;
	}
}

ThreadPool::~ThreadPool()
{
	//如果已经调用过了, 则直接返回
	if (quit == true)
		return;

	ready.lock();
	quit = true;
	if (counter > 0)
	{
		//对于处于等待状态, 则给他们发送通知,
		//这些处于等待状态的线程, 则会接收到通知,
		//然后直接退出
		if (idle > 0)
			ready.broadcast();

		//对于正处于执行任务的线程, 他们接收不到这些通知,
		//则需要等待他们执行完任务
		while (counter > 0)
			ready.wait();
	}
	ready.unlock();
}
