/*
 * Customer.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "EpochStopwatch.h"

// customers arrive to the system every X seconds between these limits
#define C_LOWB			60
#define C_UPPB			240
#define MIN_CUST_TIME	30
#define MAX_CUST_TIME	360

class Customer {
private:
	int customer_ID;
	int enter_queue;
	int leave_queue;
	int start_teller;
	int end_teller;
	EpochStopwatch* p_esw;

public:
	void enterQueue();
	void leaveQueue();
	void startTeller();
	void endTeller();

	int getEnterQueue();
	int getLeaveQueue();
	int getStartTeller();
	int getEndTeller();

	int getID();

	Customer(int, EpochStopwatch*);
	virtual ~Customer();
};

#endif /* CUSTOMER_H_ */
