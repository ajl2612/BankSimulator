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


Customer::Customer(int id, EpochStopwatch* p_epoch) {
	this-> customer_ID = id;
	this->p_esw = p_epoch;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

void Customer::enterQueue(){
	enter_queue = p_esw->getSecsSinceEpoch();
}

void Customer::leaveQueue(){
	leave_queue = p_esw->getSecsSinceEpoch();
}

void Customer::startTeller(){
	start_teller = p_esw->getSecsSinceEpoch();
}

void Customer::endTeller(){
	end_teller = p_esw->getSecsSinceEpoch();
}

int Customer::getEnterQueue(){
	return enter_queue;
}

int Customer::getLeaveQueue(){
	return leave_queue;
}

int Customer::getStartTeller(){
	return start_teller;
}

int Customer::getEndTeller(){
	return end_teller;
}

int Customer::getID(){
	return customer_ID;
}

