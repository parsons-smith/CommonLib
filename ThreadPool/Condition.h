#ifndef CONDITION_H_INCLUDED
#define CONDITION_H_INCLUDED

#include <pthread.h>
#include <time.h>

class Condition
{
public:
	Condition(const pthread_mutexattr_t *mutexAttr = NULL,
			  const pthread_condattr_t  *condAttr = NULL);
	~Condition();

	//条件变量函数
	int signal();
	int broadcast();
	int wait();
	int timedwait(int seconds);

	//互斥量函数
	int lock();
	int trylock();
	int unlock();

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
};

#endif // CONDITION_H_INCLUDED
