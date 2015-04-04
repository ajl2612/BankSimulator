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
#include "Teller.h"
#include "Securityguard.h"

#define TELL_RESEND_DELAY	1

class Teller;
class Securityguard;

class CustomerQueue : public Actor {
private:
	pthread_mutex_t* mutex;
	std::queue<Customer*> p_cQueue;

public:
	int maxDepth;
	int numTellers;
	int numTellersClosed;
	bool timeToClose;

	Teller** p_tellers;
	Securityguard* p_guard;


	CustomerQueue(pthread_mutex_t*, int);
	virtual ~CustomerQueue();

	bool empty();
	int size();
	Customer* front();
	void pop();
	void push(Customer*);
	int getMaxDepth();

	static void *runProcess(void*);
};

#endif /* CUSTOMERQUEUE_H_ */
