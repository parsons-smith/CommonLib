#include "Condition.h"

Condition::Condition(const pthread_mutexattr_t *mutexAttr,
					 const pthread_condattr_t  *condAttr)
{
	//初始化互斥量
	pthread_mutex_init(&m_mutex, mutexAttr);
	//初始化条件变量
	pthread_cond_init(&m_cond, condAttr);
}
Condition::~Condition()
{
	//销毁互斥量
	pthread_mutex_destroy(&m_mutex);
	//销毁条件变量
	pthread_cond_destroy(&m_cond);
}
int Condition::signal()
{
	return pthread_cond_signal(&m_cond);
}
int Condition::broadcast()
{
	return pthread_cond_broadcast(&m_cond);
}
int Condition::wait()
{
	return pthread_cond_wait(&m_cond, &m_mutex);
}
int Condition::timedwait(int seconds)
{
	//获取当前时间
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	//将当前时间加上需要等待的秒数, 构成绝对时间值
	abstime.tv_sec += seconds;
	return pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
}

int Condition::lock()
{
	return pthread_mutex_lock(&m_mutex);
}
int Condition::trylock()
{
	return pthread_mutex_trylock(&m_mutex);
}
int Condition::unlock()
{
	return pthread_mutex_unlock(&m_mutex);
}
