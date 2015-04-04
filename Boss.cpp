/*
 * Boss.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: ajl2612
 */

#include "Boss.h"

Boss::Boss(){
	// default constructor
}

Boss::~Boss() {
	// default destructor
}


void *Boss::runProcess(void *threadid){

	Boss *self;
	self = (Boss *) threadid;

	printf("### START RUN PROCESS BOSS\n");
	printf("BOSS           : DAY'S ALREADY STARTED, GET TO WORK!\n");

	self->p_guard->sendMessage(SEC_OPEN_BANK);

	int hours,minutes;
	for(hours = 0; hours < 7; hours++){
		for(minutes=0; minutes < 60; minutes++){
			usleep(1*MINUTES_T);
		}
	}

	printf("BOSS           : DAY'S OVER. FINISH YOUR WORK THEN GET OUT OF HERE!\n");
	self->p_guard->sendMessage(SEC_SIGNAL_CLOSE);

	printf("### END RUN PROCESS BOSS\n");
	pthread_exit(NULL);
}
