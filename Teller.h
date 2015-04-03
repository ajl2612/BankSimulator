/*
 * Teller.h
 *
 *  Created on: Apr 2, 2015
 *      Author: ajl2612
 */

#ifndef TELLER_H_
#define TELLER_H_

#include "Customer.h"
#include "Actor.h"

class Teller : public Actor{
private:
	Customer* p_cust;
	time_t lastCust;
	double workTime;

public:
	Teller(time_t*);
	virtual ~Teller();
	void setCustomer( Customer* );
	static void *runProcess(void*);
};

#endif /* TELLER_H_ */
