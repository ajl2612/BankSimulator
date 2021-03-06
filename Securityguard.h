/*
 * Securityguard.h
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#ifndef SECURITYGUARD_H_
#define SECURITYGUARD_H_

class CustomerQueue;

#include "Actor.h"
#include "CustomerQueue.h"
#include "EpochStopwatch.h"

class Securityguard : public Actor {
public:
	int numCustomers;
	bool timeToClose;
	Customer** p_customers;
	CustomerQueue* p_custQueue;
	pthread_mutex_t* mutex;
	EpochStopwatch* p_esw;

	Securityguard(pthread_mutex_t*);
	virtual ~Securityguard();
	static void *runProcess(void*);
	int getNumCustomers();
};

#endif /* SECURITYGUARD_H_ */
