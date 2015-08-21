/*
 * CASQueue.h
 *
 *  Created on: 2015Äê8ÔÂ19ÈÕ
 *      Author: Shen
 */

#ifndef CASQUEUE_H_
#define CASQUEUE_H_

template <class T>
struct NODE{
	NODE<T> *next;
	T data;
};

template <class T>
class CASQueue {
public:
	CASQueue();
	virtual ~CASQueue();
	void push(T e);
	T pop();
	T front_element();
	T back_element();
	int getSize();
	bool isEmpty();
	void print();
private:
	NODE<T> * front;
	NODE<T> * rear;

};

#endif /* CASQUEUE_H_ */
