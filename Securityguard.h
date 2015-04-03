/*
 * Securityguard.h
 *
 *  Created on: Apr 1, 2015
 *      Author: ajl2612
 */

#ifndef SECURITYGUARD_H_
#define SECURITYGUARD_H_

#include "Actor.h"
#include "CustomerQueue.h"

class Securityguard : public Actor {
public:
	int numCustomers;
	Customer** customers;
	CustomerQueue* p_custQueue;
	pthread_mutex_t* mutex;

	Securityguard(time_t*, CustomerQueue*, pthread_mutex_t*, Customer** );
	virtual ~Securityguard();
	int getNumCustomers();
	Customer* getFirstCustomer();

	static void *runProcess(void*);
};

#endif /* SECURITYGUARD_H_ */
