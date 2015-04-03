/*
 * Customer.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#include "Customer.h"

Customer::Customer() {
	this-> customer_ID = -1;
}


Customer::Customer(int id) {
	this-> customer_ID = id;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer::enterQueue(){
	time(&enter_queue);
}

void Customer::leaveQueue(){
	time(&leave_queue);
}

void Customer::startTeller(){
	time(&start_teller);
}

void Customer::endTeller(){
	time(&end_teller);
}

time_t Customer::getEnterQueue(){
	return enter_queue;
}

time_t Customer::getLeaveQueue(){
	return leave_queue;
}

time_t Customer::getStartTeller(){
	return start_teller;
}

time_t Customer::getEndTeller(){
	return end_teller;
}

int Customer::getID(){
	return customer_ID;
}

