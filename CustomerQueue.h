/*
 * CustomerQueue.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#ifndef CUSTOMERQUEUE_H_
#define CUSTOMERQUEUE_H_

#include "Customer.h"
#include "Actor.h"

class CustomerQueue : public Actor {
private:
	pthread_mutex_t* mutex;
	std::queue<Customer*> p_cQueue;

public:

	int maxDepth;
	int numTellers;


	CustomerQueue(time_t*, pthread_mutex_t*);
	virtual ~CustomerQueue();

	bool empty();
	int size();
	Customer* front();
	void pop();
	void push(Customer*);
	int getMaxDepth();

	pthread_mutex_t* getMutex();

	static void *runProcess(void*);
};

#endif /* CUSTOMERQUEUE_H_ */
