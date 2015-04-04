/*
 * Actor.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: ajl2612
 */

#include "Actor.h"


Actor::Actor() {
	this->shouldRun = true;
}

Actor::~Actor() {
	// default destructor
}

void Actor::sendMessage(int i){
	this->msgQueue.push(i);
}
