/*
 * Teller.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: ajl2612
 */

#include "Teller.h"


Teller::Teller(time_t* p_time) : Actor(p_time) {

}

Teller::~Teller() {

}

Customer* Teller::setCustomer(Customer* p_newCust){
	this->p_cust = p_newCust;
}
