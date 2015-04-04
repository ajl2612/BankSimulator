/*
 * Teller.h
 *
 *  Created on: Apr 2, 2015
 *      Author: ajl2612
 */

#ifndef TELLER_H_
#define TELLER_H_

#define BREAK_MIN_T			1800
#define BREAK_MAX_T			3600
#define CUSTOMER_MIN_T		30
#define CUSTOMER_MAX_T		360
#define	NUM_EXCUSES 		80

class CustomerQueue;
class BreakStopwatch;
class Stopwatch;

#include "Customer.h"
#include "Actor.h"
#include "CustomerQueue.h"
#include "BreakStopwatch.h"
#include "Stopwatch.h"
#include <string>

#define MIN_TEL_BREAK	60
#define MAX_TEL_BREAK	240
#define NUM_EXCUSES		80

class Teller : public Actor{
public:
	int id;
	bool timeForBreak;

	Customer* p_cust;
	CustomerQueue* p_custQueue;
	BreakStopwatch* p_bsw;
	Stopwatch* p_ssw;
	static const string excuses[];

	Teller(int);
	virtual ~Teller();
	void setCustomer( Customer*);
	static void *runProcess(void*);
};

#endif /* TELLER_H_ */
