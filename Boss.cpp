/*
 * Boss.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: ajl2612
 */

#include "Boss.h"

Boss::Boss(time_t* p_time, Securityguard* p_secguard): Actor( p_time )  {
	this->p_guard = p_secguard;
}

Boss::~Boss() {
	// TODO Auto-generated destructor stub
}


void *Boss::runProcess(void *threadid){

	printf("### START RUN PROCESS : BOSS\n");

	Boss *self;
	self = (Boss *) threadid;

	printf("BOSS: DAY'S ALREADY STARTED, GET TO WORK!\n");

	self->p_guard->sendMessage(SEC_OPEN_BANK);
	printf("BOSS: told guard\n");
	int hours,minutes;
	for(hours = 0; hours < 7; hours++){
		for(minutes=0; minutes < 60; minutes++){
			usleep(1*MINUTES_T);
		}
	}

	self->p_guard->sendMessage(SEC_SIGNAL_CLOSE);

	printf("BOSS: DAY'S OVER. FINISH YOUR WORK THEN GET OUT OF HERE!\n");
	printf("### END RUN PROCESS : BOSS\n");
	pthread_exit(NULL);
}
