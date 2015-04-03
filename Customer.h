/*
 * Customer.h
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <time.h>	//for using time objects


// customers arrive to the system every X minutes between these limits
#define C_LOWB	1
#define C_UPPB	4


class Customer {
private:
	int customer_ID;
	time_t enter_queue;
	time_t leave_queue;
	time_t start_teller;
	time_t end_teller;
public:
	void enterQueue();
	void leaveQueue();
	void startTeller();
	void endTeller();

	time_t getEnterQueue();
	time_t getLeaveQueue();
	time_t getStartTeller();
	time_t getEndTeller();

	int getID();

	Customer();
	Customer(int);
	virtual ~Customer();
};

#endif /* CUSTOMER_H_ */
