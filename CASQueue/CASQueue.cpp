/*
 * CASQueue.cpp
 *
 *  Created on: 2015Äê8ÔÂ19ÈÕ
 *      Author: Shen
 */

#include "CASQueue.h"

#include <iostream>
using namespace std;

template <class T>
CASQueue<T>::CASQueue() {
	// TODO Auto-generated constructor stub
	NODE<T> *p = new NODE<T>;
	if(p == NULL){
		cout<<"ERROR:Failed to malloc the node."<<endl;
	}
	p->data = NULL;
	p->next = NULL;
	front = p;
	rear = p;

}

template <class T>
CASQueue<T>::~CASQueue() {
	// TODO Auto-generated destructor stub
	while(!isEmpty()){
		pop();
	}
}

template <class T>
void CASQueue<T>::push(T e)
{
	NODE<T>* q = new NODE<T>;
	if (NULL == q){
		cout << "ERROR:Failed to malloc the node." << endl;
	}
	q->data = e;
	q->next = NULL;
	NODE<T> *p = rear;
	NODE<T> *oldp = p;

	do{
		while(p->next != NULL)
			p = p->next;
	}while(__sync_val_compare_and_swap(&(p->next), NULL, q) != NULL);
	__sync_val_compare_and_swap(&rear, oldp, q);
}

template <class T>
T CASQueue<T>::pop()
{
	NODE<T> *p;
	do{
		p = front->next;
		if(p == NULL){
			return NULL;
		}
	}while(__sync_val_compare_and_swap(&(front->next), p ,p->next) == NULL);
	return p->data;
}

template <class T>
T CASQueue<T>::front_element()
{
	if (front == rear){
		return NULL;
	}else{
		NODE<T>* p = front->next;
		return p->data;
	}
}

template <class T>
T CASQueue<T>::back_element()
{
	if (front == rear){
		return NULL;
	}else{
		return rear->data;
	}
}

template <class T>
int CASQueue<T>::getSize()
{
	int count(0);
	NODE<T>* p = front;
	while (p != rear){
		p = p->next;
		count++;
	}
	return count;
}

template <class T>
bool CASQueue<T>::isEmpty()
{
	if (front == rear){
		return true;
	}else{
		return false;
	}
}

template <class T>
void CASQueue<T>::print()
{
	NODE<T>* p = front;
	while (p != rear){
		p = p->next;
		cout<<p->data<<endl;
	}
}
