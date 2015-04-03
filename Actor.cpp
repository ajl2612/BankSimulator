/*
 * Actor.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#include "Actor.h"


Actor::Actor(time_t* p_time) {
	this->epoch = *p_time;

}

Actor::~Actor() {
	// TODO Auto-generated destructor stub
}

void Actor::sendMessage(int i){
	this->msgQueue.push(i);
}
