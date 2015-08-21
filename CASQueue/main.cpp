/*
 * main.cpp
 *
 *  Created on: 2015年8月20日
 *      Author: hunt
 */
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "CASQueue.cpp"
using namespace std;
CASQueue<int> flq;

void *producert(void *arg){
	for(int i = 0; i < 100 ; i++){
		//usleep(10);
		cout<<"push:"<<i<<endl;
		flq.push(i);
	}
}

void *comsumert(void *arg){
	usleep(1);
	for(int i=0 ; i<100 ; i++){
		//usleep(10);
		cout<<"pop:"<<flq.pop()<<endl;
	}
}

int main(){

	pthread_t producer,comsumer;
	int l = pthread_create(&producer,NULL,producert, NULL);
	int p = pthread_create(&comsumer,NULL,comsumert, NULL);
	//cout<<l<<"----"<<p<<endl;
	pthread_join(producer,NULL);
	pthread_join(comsumer,NULL);
	return 0;
}




